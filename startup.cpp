#include <iostream>
    using std::cout;
#include <stdio.h>

//this will be the automated process of configuring OpenMPI

int main(){
    //dependency
    FILE* archOutput = popen("uname -m", "r");
    char arch[15];
    char arm[15] = "Armvl7"; //fix this
    if(archOutput != nullptr){
        fgets(arch, sizeof(arch), archOutput);
        if(strcmp(arch, arm) == 0){
            system("sudo apt install zlib1g-dev"); //no mac is arm, so no need to check
        }
    }
    pclose(archOutput);

    //determine operating system
    FILE* OSOutput = popen("uname -s", "r");
    char os[15];
    char mac[15] = "Darwin\n";
    if(OSOutput != nullptr){
        fgets(os, sizeof(os), OSOutput);
        cout << os; //for debugging
    }else{
        cout << "Error with output" << endl; //for debugging
    }
    pclose(OSOutput);

    system("mkdir ~/openmpi");
    system("cp /opt/tasks/openmpi-4.1.1.tar.gz ~/openmpi");
    system("tar zxvf openmpi-4.1.1.tar.gz");
    system("cd openmpi-4.1.1");
    system("configure --prefix=$HOME/openmpi --with-hwloc=internal --with-libevent=internal --enable-heterogeneous");
    system("sudo make all && sudo make install");
    //how to prompt to ask for password

    //update environment variables
    system("perl -pi -e 'print \"export PATH=$PATH ~/openmpi/bin\\n\" if $. == 1' ~/Desktop/thesis/env.txt");
    system("perl -pi -e 'print \"export LD_LIBRARY_PATH= ~/openmpi/lib\\n\" if $. == 1' ~/Desktop/thesis/env.txt");

    //install/enable ssh
    if(strcmp(os, mac) == 0){ //mac system
        system("sudo systemsetup -setremotelogin on");
    }else{
        system("sudo apt-get install ssh"); //linux
    }

    return 0;
}





/*

*/