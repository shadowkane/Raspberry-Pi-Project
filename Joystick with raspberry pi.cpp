#include <iostream>
#include <wiringPi.h>
using namespace std;

//we are going to display val on led using the binary format
//convert int to bit
void int2bit(int val){
    //we have 4 bits
    for(int i=0;i<5;i++){
        // we need to shift the value 'val' i times to the right and than execute the bitwise operator AND "&" so we get only the bit in the i position ( 0 or 1)
        //now we call the digitalWrite function
        digitalWrite(21+i,(val>>i)&1); //this is how we will display val on leds
    }
}

int main()
{
    wiringPiSetup();

    pinMode(0,INPUT); // up
    pinMode(1,INPUT); // mid
    pinMode(2,INPUT); //down
    pinMode(3,INPUT); //left
    pinMode(4,INPUT); //right

    pinMode(21,OUTPUT);
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);
    pinMode(25,OUTPUT);

    digitalWrite(21,0);
    digitalWrite(22,0);
    digitalWrite(23,0);
    digitalWrite(24,0);
    digitalWrite(25,0);

    //work with edges
    bool right_edge=false;
    bool left_edge=false;
    bool up_edge=false;
    bool down_edge=false;
    bool mid_edge=false;

    int val=0;
    while(1){
        //right
        if(digitalRead(4)==1 && right_edge==false){
            val++;
            right_edge=true;
        }else if(digitalRead(4)==0 && right_edge==true){
            right_edge=false;
        }
        //left
         if(digitalRead(3)==1 && left_edge==false){
            val--;
            left_edge=true;
        }else if(digitalRead(3)==0 && left_edge==true){
            left_edge=false;
        }
        //up
         if(digitalRead(0)==1 && up_edge==false){
            val++;
            up_edge=true;
        }else if(digitalRead(0)==0 && up_edge==true){
            up_edge=false;
        }
        //down
         if(digitalRead(2)==1 && down_edge==false){
            val--;
            down_edge=true;
        }else if(digitalRead(2)==0 && down_edge==true){
            down_edge=false;
        }
        //mid
         if(digitalRead(1)==1 && mid_edge==false){
            val=0;
            mid_edge=true;
        }else if(digitalRead(1)==0 && mid_edge==true){
            mid_edge=false;
        }

        //we have 4 directions like 4 bits , from 0000 to 1111 and the mid button will reset the counter
        //in case the val become less than 0 we need to reset it
        if(val>=16 || val<0){
            val=0;
        }

        int2bit(val);


    }
}
