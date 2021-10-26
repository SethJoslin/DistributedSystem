#include <mpi.h>
#include <iostream>


int main(int argc, char *argv[]){
    //initialize MPI
    MPI_Init(&argc, &argv);
    int  nodes, rank;
    //get number of nodes
    MPI_Comm_size(MPI_COMM_WORLD, &nodes);
    //get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* * * * * * * * * * MASTER * * * * * * * * * */
    if(rank == 0){
        int sumNum = 1000;
        printf("Begining Program...\n");
        
        int elPerNode = sumNum / (nodes - 1); //minus one so one is master
        int elRemainder = sumNum % (nodes - 1); //minus one so one is master
        printf("Each node will get %d elements. \n", elPerNode);
        printf("There will be %d nodes with an extra element. \n", elRemainder);

        int lowerLim;
        int upperLim = 0; //first node will deal with remainder, if present
        int limits[2];

        //sending for loop, i is being used to send to specific nodes
        for(int i = 1; i < nodes; i++){
            lowerLim = upperLim + 1;
            if(i <= elRemainder){
                //update limits[]
                upperLim = lowerLim + elPerNode + elRemainder - 1;
                limits[0] = lowerLim;
                limits[1] = upperLim;
                //send larger amount (elPerNode + elRemainder)
                MPI_Send(limits, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
            }else{
                //update limits[]
                upperLim = lowerLim + elPerNode - 1;
                limits[0] = lowerLim;
                limits[1] = upperLim;
                //send smaller amount (elPerNode)
                MPI_Send(limits, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
        //recieve data back
        int results[nodes - 1];
        int sum = 0;
        for(int i = 1; i < nodes; i++){ //this could be inefficent if an early node takes longer, very risky
            MPI_Recv(&results[i - 1], 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //add up together 
            sum += results[i - 1];
        }
        
        printf("\nThe final summation is %d!!!!!\n", sum);
        // k = 1000, answer is 500500
    }
    /* * * * * * * * * * END MASTER * * * * * * * * * */


    /* * * * * * * * * * SLAVE * * * * * * * * * */
    if(rank != 0){
        int limits[2];
        //recv array
        MPI_Recv(limits, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //calculate
        int lowerLim = limits[0];
        int upperLim = limits[1];
        int sum = lowerLim;
        while(lowerLim < upperLim){
            lowerLim++;
            sum += lowerLim;
        }
        //send data back
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    /* * * * * * * * * * END SLAVE * * * * * * * * * */

    MPI_Finalize();
    return 0;
}
