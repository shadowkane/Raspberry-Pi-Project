#include <iostream>
#include <wiringPi.h>

using namespace std;

// now the object will shoots so we need to define the position of the fire gun with a number, "2 for example"
int Object[8][8]{
{0,0,0,0,0,0,0,0},
{0,0,1,1,1,0,0,0},
{0,0,0,2,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};

//in this array we will play the fire
int Fire[8][8]{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};

//in this array we will display enemies
int Enemy[8][8]{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};

// in this array we will add all frames together
int Frame[8][8]{
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};

//** edges for only one mouve each click
bool up_edge=false;
bool down_edge=false;
bool right_edge=false;
bool left_edge=false;
bool mid_edge=false;

int fire_pos=8; // this position will control the number of transitions until the fire reach the top
int speed=0;
int speed_fire=5;

int enemy_limit=2;// the limit of enemies for each row

// the speed of launching an enemies
int enemy_launcher_speed_counter=0;
int enemy_launcher_speed=100;

// the speed of an enemy
int enemy_speed_counter=0;
int enemy_speed=100;

// level is the distance between 2 waves of enemies
int level_counter=0;
int level=2;

// score
int score=0;
//********** Functions **********//
//*** next_column
void next_col(int data){
    digitalWrite(29,data);
    digitalWrite(5,1);
    digitalWrite(6,0);
    digitalWrite(29,0);
    digitalWrite(5,0);
    digitalWrite(6,1);
}
//*** display matrix
void display_matrix(int matrix[8][8]){
    for(int i=0;i<8;i++){
        if(i==0)
            next_col(1);
        else
            next_col(0);
        for(int j=0;j<8;j++){
            digitalWrite(21+j,matrix[i][j]);
        }
        delay(1);
    }
}

//*** object movements
//* up
void  object_up(){
    // movements limits
    for(int j=0;j<8;j++)
        if(Object[7][j]>0)
            return;
    //if the object free to move, it will go up
    int accum[8];
    for(int j=0;j<8;j++)
        accum[j]=Object[7][j];

    for(int i=7;i>0;i--){
        for(int j=0;j<8;j++){
            Object[i][j]=Object[i-1][j];
        }
    }

    for(int j=0;j<8;j++)
        Object[0][j]=accum[j];
}

//* down
void  object_down(){
    // movements limits
    for(int j=0;j<8;j++)
        if(Object[0][j]>0)
            return;
    //if the object free to move, it will go up
    int accum[8];
    for(int j=0;j<8;j++)
        accum[j]=Object[0][j];

    for(int i=0;i<7;i++){
        for(int j=0;j<8;j++){
            Object[i][j]=Object[i+1][j];
        }
    }

    for(int j=0;j<8;j++)
        Object[7][j]=accum[j];
}

//* right
void object_right(){
    // movements limits
    for(int i=0;i<8;i++)
        if(Object[i][0]>0)
            return;
    //if the object free to move, it will go up
    int accum[8];
    for(int i=0;i<8;i++)
        accum[i]=Object[i][0];

    for(int i=0;i<8;i++){
        for(int j=0;j<7;j++){
            Object[i][j]=Object[i][j+1];
        }
    }

    for(int i=0;i<8;i++)
        Object[i][7]=accum[i];
}

//* left
void object_left(){
    // movements limits
    for(int i=0;i<8;i++)
        if(Object[i][7]>0)
            return;
    //if the object free to move, it will go up
    int accum[8];
    for(int i=0;i<8;i++)
        accum[i]=Object[i][7];

    for(int i=0;i<8;i++){
        for(int j=7;j>0;j--){
            Object[i][j]=Object[i][j-1];
        }
    }

    for(int i=0;i<8;i++)
        Object[i][0]=accum[i];
}

//*** Fire
//* fire launcher
void fire_launcher(){
// we nee to look for the position of the fire gun '2'
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(Object[i][j]==2){
                Fire[i][j]=1;
                fire_pos=i;
                return;
            }
        }
    }
}
//** fire direction
void fire_direction(){
    for(int i=7;i>0;i--){
        for(int j=0;j<8;j++){
            Fire[i][j]=Fire[i-1][j];
        }
    }
    for(int j=0;j<8;j++){
        Fire[0][j]=0;
    }
}

//*** enemy
//* enemy launcher
void enemy_launcher(){

    int nbr=0; // this will count the number of enemies
    for(int j=1;j<7;j++){
        Enemy[7][j]=(rand()%2)*3; // a enemy will be displayed at a random position with a number 3
        if(Enemy[7][j]>0)
            nbr++;
        if(nbr>=enemy_limit)
            return;
    }
}
//* enemy direction
void enemy_direction(){
    for(int i=0;i<7;i++){
        for(int j=0;j<8;j++){
            Enemy[i][j]=Enemy[i+1][j];
        }
    }
    for(int j=0;j<8;j++)
        Enemy[7][j]=0;
}

//* object shoots the enemy
void enemy_fire(){
    for(int i=0;i<7;i++){
        for(int j=0;j<8;j++){
            if(Enemy[i][j]+Fire[i][j]==4){ //3 for enemy + 1 for the fire
                Enemy[i][j]=0;
                Fire[i][j]=0;
                // both 0 because the shoot killed the enemy
                //display score
                score++;
                cout<<"your score now = "<<score<<endl;
            }
        }
    }

}
//** enemy kill the object
void enemy_object(){
    for(int i=0;i<7;i++){
        for(int j=0;j<8;j++){
            if(Enemy[i][j]+Object[i][j]>=4){ //3 for enemy + 1 or 2 for the object
                cout<<"Game over"<<endl;
            }
        }
    }
}
//*** frames assembler
void assembler(){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(Object[i][j]+Fire[i][j]+Enemy[i][j]>0)
                Frame[i][j]=1;
            else
                Frame[i][j]=0;
        }
    }
}
int main()
{
    wiringPiSetup();

    // for the joystick
    pinMode(0,INPUT); // right
    pinMode(1,INPUT); // left
    pinMode(2,INPUT); // mid
    pinMode(3,INPUT); // up
    pinMode(4,INPUT); // down

    //for the led matrix
        // 74hc595
    pinMode(29,OUTPUT); //data
    pinMode(5,OUTPUT); //SH_CP
    pinMode(6,OUTPUT); //ST_CP
        //uln2803
    pinMode(21,OUTPUT);
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(28,OUTPUT);

    //initialization
    for(int i=021;i<29;i++){
        digitalWrite(i,0);
    }

    while(1){

        // go up
        if(digitalRead(3)==1 && up_edge==false){
            object_up();
            up_edge=true;
        }else if(digitalRead(3)==0 && up_edge==true){
            up_edge=false;
        }
        // go down
        if(digitalRead(4)==1 && down_edge==false){
            object_down();
            down_edge=true;
        }else if(digitalRead(4)==0 && down_edge==true){
            down_edge=false;
        }
        // go right
        if(digitalRead(0)==1 && right_edge==false){
            object_right();
            right_edge=true;
        }else if(digitalRead(0)==0 && right_edge==true){
            right_edge=false;
        }
         // go left
        if(digitalRead(1)==1 && left_edge==false){
            object_left();
            left_edge=true;
        }else if(digitalRead(1)==0 && left_edge==true){
            left_edge=false;
        }
         // mid launche fire
        if(digitalRead(2)==1 && mid_edge==false){
            fire_launcher();
            mid_edge=true;
        }else if(digitalRead(2)==0 && mid_edge==true){
            mid_edge=false;
        }

        if(fire_pos<8){
            speed++;
            if(speed>=speed_fire){
                speed=0;
                fire_direction();
                fire_pos++;
            }
        }

        // enemies
        enemy_launcher_speed_counter++;
        if(enemy_launcher_speed_counter>=enemy_launcher_speed){
            enemy_launcher_speed_counter=0;
            level_counter++;
            if(level_counter>=level){
                enemy_launcher();
                level_counter=0;
            }
        }

        enemy_speed_counter++;
        if(enemy_speed_counter>=enemy_speed){
            enemy_speed_counter=0;
            enemy_direction();
        }

        enemy_fire();
        enemy_object();
        assembler();
        display_matrix(Frame);
    }

    return 0;
}
