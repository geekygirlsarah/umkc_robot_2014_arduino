
/*
 * navigation
 * top level functions for navigating
 * look at the state diagram
 * 2014 umkc robotics 
 */


#include <AnalogDistanceSensor.h>
#include <Distance2D120X.h>
#include <DistanceSensor.h>

#include <gapfinder.h>
#include <irsensor_tester.h>
#include <magellan_edgesensors.h>
#include <fronteyes.h>
#include <parallelpark.h>

#include <motor_cmd.h>

#include <movement.h>

#ifndef NAVIGATION_H
#define NAVIGATION_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

class Navigation {
    private:
                motor_cmd sabertooth;
                FrontEyes eyes;
                movement mov;
                GapFinder gapfind;
                QuadEncoder encoders;
                ParallelPark par; 
                Magellan mag;
                
                const static int32_t ticksFor90 = 3450;
                bool goingForward;
    public:
                void init()  {
                  sabertooth.begin(2);        //motor controller on serial2
                  encoders.init();
                  mov.init(&sabertooth);
                  Serial.println("ready");
                  gapfind.init(A0,A1,A2);
                  eyes.init(A3,15);
                  mag.init(A6,A7);
                  par.init(A2,A1,A0);
                  
                  goingForward = true;  //... this is from the point of view of robot. it will always start going forward.
                                        //it's just that the motor library has it as "reverse()" as our going forward XD
                }
                
                
                //Stuff that needs to be updated goes in here.
                //NO NO NO NO NONONONONONONO
                //update-y things need to GO IN THE THING ITSELF BLAHHHHHHHHH or be called in the nav functions themselves nonono
                /*
                void update()  {
                //keep going forward until you find gap
                    if(!gapfind.gapPresent())
                      gapfind.update();  
                    //gapfind.printGapStatus();
                    //gapfind.printDebug();
                    //sabertooth.forward();
                                
                    eyes.update();
                    //if(eyes.obstaclePresent())
                    //   sabertooth.all_stop();
                
                } 
                */
                
                void takeOff()  {
                  sabertooth.reverse(20);
                  goingForward = true;
                }
                
                //toggle in forward or backwards when magellans see seomthing
                void traveling()  {
                   mag.printDebug();
                   //mag.printDebugDifference();
                   mag.printEdgeStatus();
                   mag.update();
                   //if it senses ANYTHING reverse direction quick!
                  /*
                  //twitches and just twitches forward and backward
                   if( !mag.isFrontSafe() || !mag.isBackSafe())  {
                     Serial.println("ACK SOMETHING!");
                     sabertooth.all_stop();
                     delay(500);
                     goingForward = !goingForward;
                   } 
                   */
                   
                   if(!mag.isFrontSafe() && goingForward)  {
                     Serial.println("ACK SOMETHING!");
                     sabertooth.all_stop();
                     delay(500);
                     goingForward = false;
                     
                  
                   }
                   if(!mag.isBackSafe() && !goingForward)  {
                     Serial.println("ACK SOMETHING!");
                     sabertooth.all_stop();
                     delay(500);
                     goingForward = true;
                   }
                   if(goingForward)  {
                     Serial.println("go forward");
                     sabertooth.reverse(20);
                   }
                   else  {
                     Serial.println("go reverse");
                     sabertooth.forward(20);
                   }
                }
                
                //returns if gap is found.
                bool lookingForGap()  {
                  sabertooth.reverse(20);
                  //sabertooth.forward(20);
                  //console.println("moving \t checking gap");
                  //check if a gap has been found
                  gapfind.printDebug();
                  gapfind.printGapStatus();
                  gapfind.update();
                  
                  
                  //gap found? move forward a set amount to center self
                  if(gapfind.gapPresent())  {
                    //Serial.println("moving \t GAP FOUND!!");
      
                    
                    int32_t start_ticks = positionFL;
                    while(true)  {
                      if(abs(positionFL - start_ticks) > 500)
                         break; 
                     }
                    sabertooth.all_stop();
                    return true;
                  }
                  return false;
                }
                
                void turnTowardsLane()  {
                  mov.turn90degreesCW(0x60,0x20,ticksFor90);
        
                  sabertooth.all_stop();
                  delay(500);
                }
                
                void turnTowardsGap()  {
                  mov.turn90degreesCW(0x20,0x60,ticksFor90);
        
                  sabertooth.all_stop();
                  delay(500);
                }
                
                bool crossGap()  {
                  
                  sabertooth.reverse(20);
                   eyes.update();
                   eyes.printDebug();
                   if(eyes.obstaclePresent())  {
                     sabertooth.all_stop();
                     Serial.println("dont crash");
                     return true;
                   }
                   return false;
                }
                
                void parallelpark()  {
                
                  
                  //WHOAAAAH
                  //change of paradigm.
                  //instead of going until you stop, you should stop a bit and then go if it's bad. so default to stopping
                  
                  mov.turn(0x60,0x20);
                  while(!par.isParallel())  {
                    mov.turn(0x60,0x20);
                    //keep going
                    //par.printDebug();
                    par.printStatus();
                    par.update();
                  }
                  sabertooth.all_stop();
               
       
                }
                void sleep()  {
                  sabertooth.all_stop();
                  delay(500);
                }
               
                
		
			
};




#endif
