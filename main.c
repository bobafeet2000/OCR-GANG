#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "source/sdl/our_sdl.h"
#include "source/segmentation/segmentation.h"
#include "source/process/process.h"
#include "source/matrix/matrix.h"
#include "source/sigmoid/sigmoid.h"
#include "err.h"


int main(int argc, char** argv) {
    if (argc<2){
        printf("-----------------------\n");
        printf("Bienvenue dans OCR GANG\n");
        printf("-----------------------\n");
        printf("Arguments :\n");
        printf("    --seg   Montre la segmentation (spécifiez un image path)\n");
        printf("    --XOR   Montre la fonction XOR\n");
        return 0;
    }
    if (strcmp(argv[1], "--seg") == 0){
        init_sdl(); /* Init SDL */

        /* Load Image */
        SDL_Surface* image=load_image(argv[2]);

        SDL_LockSurface(image);
        
        /* Black and White */
        image = black_and_white(image);

        DrawLines(image); //Separates into blocs delimited by red color
        int BlocNumber = NumberOfBlocs(image);
        SDL_Surface ***letters = malloc(sizeof(SDL_Surface**) * BlocNumber);
        SDL_Surface **blocs = malloc(sizeof(SDL_Surface*) * BlocNumber);

        DivideIntoBlocs(image,blocs,letters); //Divides image into lines

        char file[12];
        for (int j = 0; j < BlocNumber; ++j) {
            for (int i = 0; i < NumberOfLetters(blocs[j]); ++i) {
                char loc[100] = "img/temp/";
                sprintf(file,"%i",j*100 + i);
                strcat(file,".bmp");
                SDL_SaveBMP(letters[j][i], strcat(loc,file)); //Save blocs in folder
            }
        }

        SDL_UnlockSurface(image);

        SDL_SaveBMP(image,"new.bmp"); //Save image in folder

        /*
        SDL_Surface* screen = display_image(image);

        SDL_Event event;

        // Wait for a key to be down.
        do
        {
            SDL_PollEvent(&event);
        } while(event.type != SDL_KEYDOWN);

        // Wait for a key to be up.
        do
        {
            SDL_PollEvent(&event);
        } while(event.type != SDL_KEYUP);

        SDL_FreeSurface(screen);
        */

        SDL_Quit();
        printf("%i \n",NumberOfLetters(blocs[0]));
    }
    else{
        if(strcmp(argv[1], "--XOR")==0){
            /* Declare all we are gonna need */
            static const int number_of_inputs = 2;
            static const int number_of_hidden_nodes = 2;
            static const int number_of_outputs = 1;
            double hidden_layer[number_of_hidden_nodes];
            double output_layer[number_of_outputs];
            double hidden_layer_bias[number_of_hidden_nodes];
            double output_layer_bias[number_of_outputs];
            double hidden_weights[number_of_inputs*number_of_hidden_nodes];
            double output_weights[number_of_hidden_nodes*number_of_outputs];
            static const int number_training_sets = 4;
            FILE *result_file;
            result_file = fopen("source/Xor/Xor-data.txt", "w"); 
            double training_inputs[] = {0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f};
            double training_outputs[]= {0.0f,1.0f,1.0f,0.0f};
            int trainingSetOrder[] = {0,1,2,3};
            const double lr = 0.5f;

            if(cfileexists("source/Xor/Xor-weights.txt")&&!fileempty("source/Xor/Xor-weights.txt"))
            {
                read_file("source/Xor/Xor-weights.txt",number_of_inputs,number_of_outputs, number_of_hidden_nodes,hidden_layer_bias,hidden_weights,output_layer_bias,output_weights);
            }
            else{
                initialization(number_of_inputs,number_of_hidden_nodes,number_of_outputs,hidden_weights,hidden_layer_bias,output_weights);
            }
            printf("Finished all initialization !\n");
            char answer[1];
            printf("Do you want to train the neural network or use it ?\n1 = Train it\n2 = Use it\n");
            fgets(answer,2,stdin);
            printf("Started computing ... \n");
            if (atoi(&answer[0])== 1)
            {
                int nb = 1000;
                int step = 0;
                for (int n=0; n < nb; n++)
                {
                    step++;
                    progressBar(step,nb);
                    shuffle(trainingSetOrder,number_training_sets);
                    for (int x=0; x<number_training_sets; x++)
                    {
                        int i = forward_pass(x ,number_of_inputs,number_of_hidden_nodes,number_of_outputs,trainingSetOrder,training_inputs,hidden_weights,hidden_layer_bias,output_weights,output_layer_bias,hidden_layer,output_layer);
                        fprintf(result_file, "input : %f ^ %f => output = %f , expected : %f\n",training_inputs[i*number_of_inputs],training_inputs[i*number_of_inputs+1],output_layer[0],training_outputs[i*number_of_outputs]);
                        back_propagation(lr, i,number_of_inputs,number_of_hidden_nodes,number_of_outputs,training_inputs,training_outputs,hidden_weights,hidden_layer_bias,output_weights,output_layer_bias,hidden_layer,output_layer);
                    }
                }
                printf("\n");
                printf("\e[?25h");
                fclose(result_file);
                save_weights_bias(number_of_outputs,number_of_hidden_nodes,number_of_inputs,output_layer_bias,output_weights,hidden_layer_bias,hidden_weights);
            }
            else if (atoi(&answer[0])== 2)
            {
                printf("not implemented yet, come back later!");
                /*char input1[1];
                char input2[1];
                printf("Please input the value you want to enter.\n");
                fgets(input1,2,stdin);
                fgets(input2,2,stdin);*/
            }
            
        }
        else{
            printf("-----------------------\n");
            printf("Bienvenue dans OCR GANG\n");
            printf("-----------------------\n");
            printf("Arguments :\n");
            printf("    --seg   Montre la segmentation (spécifiez un image path)\n");
            printf("    --XOR   Montre la fonction XOR\n");
            return 0;
        }
    }
    return 0;
}



