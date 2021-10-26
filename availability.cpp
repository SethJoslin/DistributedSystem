#include <iostream>
    using std::cout;
    using std::endl;
 #include <stdio.h>
 #include <cstdlib>

//This will be ran on the nodes to update their status in the database

int main(){
    bool available = false;

    //determine operating system
    FILE* OSOutput = popen("uname -s", "r");
    char os[15];
    if(OSOutput != nullptr){
        fgets(os, sizeof(os), OSOutput);
        cout << os; //for debugging
    }else{
        cout << "Error with output" << endl; //for debugging
    }
    pclose(OSOutput);

    //'top' will get cpu utilization info
    FILE* CPUOutput = popen("iostat | grep \"\\.\"", "r");
    char cpulong[30];
    char cpu[3];
    int  intcpu;
    if(CPUOutput != nullptr){
        fgets(cpulong, sizeof(cpulong), CPUOutput);
        cpu[0] = cpulong[27];
        cpu[1] = cpulong[28];
        intcpu = atoi(cpu);
        cout << intcpu << endl; //for debugging
    }else{
        cout << "Error with output" << endl; //for debugging
    }
    pclose(CPUOutput);


    if(intcpu >= 50){ //if idle is 50% or higher
        //determine battery life
        FILE* BatteryOutput;
        char mac[15] = "Darwin\n";
        char bat[4];
        int  intbat;
        char batlong[27];
        if(strcmp(os, mac) == 0){ //mac system
            BatteryOutput = popen("pmset -g batt | grep -Eo \"\\d+%\" | cut -d\\% -f1", "r");
            fgets(bat, sizeof(bat), BatteryOutput);
            intbat = atoi(bat);
            cout << intbat << endl; //for debugging
        }else{ //not mac
            BatteryOutput = popen("upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep \"power supply:\"", "r");
            fgets(batlong, sizeof(batlong), BatteryOutput);
            char noBat[28] = "  power supply:         no\n";
            if(strcmp(batlong, noBat) == 0){ //no battery supply, desktop
                strcpy(bat, "100");
                intbat = atoi(bat);
            }else{ //battery supply, laptop
                //figure out command for outputting only the percentage
                //upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep "percentage:"
            }
        }
        pclose(BatteryOutput);
        if(intbat >= 50){ //if bat is higher than 50%
            available = true;
            cout << "True" << endl; //debugging

        }
    }




    //assimulate all info, make the string


    //ssh node@147.97.108.199 "mysql NODESDB -e 'INSERT INTO Nodes(available, nickname, ipAddress, arch, sockets) VALUES (1, '\'workermac\'', '\'147.97.110.151\'', 0, 6);'"
    //this will send the command to update the database
    return 0;
}


/*

    //determine arch, this is be used for compilation
    FILE* archOutput = popen("uname -m", "r");
    char arch[15];
    if(archOutput != nullptr){
        fgets(arch, sizeof(arch), archOutput);
        cout << arch; //for debugging
    }else{
        cout << "Error with output" << endl; //for debugging
    }
    pclose(archOutput);

*/