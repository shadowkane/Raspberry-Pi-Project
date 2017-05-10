#include <iostream>
#include <wiringPi.h>
#include <vector>

using namespace std;

struct char_index{
    char c; // this will take the letter like A B C etc
    int index; // this will take the index or the position of that letter in that chars table
};

char_index char_table[2];

int chars[2][8][8]{
//A
{
{0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,0},
{1,1,1,1,1,1,1,0},
{0,0,1,1,0,0,1,1},
{0,0,1,1,0,0,1,1},
{1,1,1,1,1,1,1,0},
{1,1,1,1,1,1,1,0},
{0,0,0,0,0,0,0,0}
},
//B
{
{0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1},
{1,0,0,1,1,0,0,1},
{1,0,0,1,1,0,0,1},
{1,1,1,1,1,1,1,1},
{0,1,1,0,0,1,1,0},
{0,0,0,0,0,0,0,0}
}
};


vector<vector<int> > str_matrix;

void build_message(vector<vector<int> >& str, string msg){
    int col=0;
    for(int i=0;i<msg.length();i++){
        int ii=0;
        while(msg[i]!=char_table[ii].c)
            ii++;
        ii=char_table[ii].index;
        for(int j=0;j<8;j++){
            for(int k=0;k<8;k++){
                str[col][k]=chars[ii][j][k];
            }
            col++;
        }

    }
}

void next_column(int data){
     digitalWrite(0,data);
     digitalWrite(1,1);
     digitalWrite(2,0);

     digitalWrite(0,0);
     digitalWrite(1,0);
     digitalWrite(2,1);
}

void show_column_char(vector<vector<int> >char_x,int pos){
    for(int i=0;i<8;i++){
        digitalWrite(i+21,char_x[pos][i]);
    }
}

int main()
{
    wiringPiSetup();

    pinMode(0,OUTPUT); // this is for data
    pinMode(1,OUTPUT); //this is for SH_CP
    pinMode(2,OUTPUT); //this is for ST_CP

    pinMode(21,OUTPUT);
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(28,OUTPUT);

    // those for column data
    int t=2;
    int again=0;

    char direction='L';// L from right to left , R from left to right
    int d=1;
    int speed=0;

    string msg="ABAAAABB";

    int matrix_length=8;
    int str_last_col=(msg.length()*8) - 1;
    int start;
    int limite;

    if(direction=='R'){
        d=-1;
        limite=8;
        start=str_last_col*(-1);
    }else{
        d=1;
        limite=str_last_col;
        start=matrix_length*(-1);
    }
    int pos=start*d;


    str_matrix.resize(msg.length()*8);
    for(int i=0;i<msg.length()*8;i++){
        str_matrix[i].resize(8);
    }

    char_table[0].c='A';
    char_table[0].index=0;

    char_table[1].c='B';
    char_table[1].index=1;

    build_message(str_matrix,msg);

    while(1){

        // this to select the right column
        if(again==0){
            next_column(1);
        }else{
            next_column(0);
        }
        again++;


        // this to display the right led for each column
        // now we will try to do some animation with shifting the letter


        if(pos<0 || pos >str_last_col){
            show_column_char(str_matrix,0);
        }else{
            show_column_char(str_matrix,pos);
        }

        pos++;

        if(again==matrix_length){
            again=0;
            pos=start*d;
            speed++;
            if(speed==10){
                speed=0;
                start++;
                if(start>=limite){
                    if(direction=='L')
                        start=matrix_length*(-1);
                    else
                        start=str_last_col*(-1);
                }
            }
        }

        delay(t);

    }
    return 0;
}
