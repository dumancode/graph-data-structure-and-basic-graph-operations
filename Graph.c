#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FURKAN DUMAN
// 2453173

struct mail
{
    int Email_ID;
    char Sender[51];
    char Receipt[51];
    int Day_of_the_month;
    int countWords;
};


struct graphHead
{
    int count;
    struct graphVertex  *firstVertex;
};

struct  graphVertex
{
    struct graphVertex *nextVertex;
    char data[51] ;
    int degree;
    int outdegree;
    int processed;
    struct graphArc *firstArc;

};

struct graphArc
{

    struct graphVertex *destination;
    struct graphArc *nextArc;
    int weight;

};


struct graphHead * createGraph(void)

{
    struct graphHead  *head =  ( struct graphHead  *) malloc(sizeof (struct graphHead));

    head->count = 0;
    head->firstVertex = NULL;
    return head;
};

void insertVertex(struct graphHead *head, char  data [51])
{
    struct graphVertex* tmp = head->firstVertex;

    int counter=0,k, check=1;

    if(head->count == 0)   //If graph is empty
    {
        check = 1;

    }

    else  // If Graph is not empty,  I'm traveling the graph, If there is a vertex with the same "data" the counter is 0 so there can't be 2 vertexes with the same name.
    {
        for(k = 0; k < head->count; k++)
        {
            if((strcmp(tmp->data, data ) == 0) )
            {

                check = 0;
                break;

            }

            tmp = tmp->nextVertex;
        }
    }

    if(check == 1)
    {


        struct graphVertex  *vertex= (struct graphVertex *) malloc(sizeof(struct graphVertex));
        vertex->nextVertex = NULL;
        vertex->firstArc=NULL;
        strcpy(vertex->data,data);
        vertex->outdegree=0;
        vertex->degree = 0;
        vertex->processed = 0;


        if(head->firstVertex == NULL)
        {
            head->firstVertex = vertex;

            head->count++;
        }

        else  //travels and find last vertex
        {
            struct graphVertex *temp = head->firstVertex;
            while(temp->nextVertex != NULL)
            {
                temp = temp->nextVertex;
            }

            temp->nextVertex = vertex;
            head->count++;
        }

    }
}

int insertArc(struct graphHead *head, char fromKey [100], char toKey [100],int weight)
{
    struct graphArc *arc = ( struct graphArc *) malloc(sizeof(struct graphArc));
    arc->destination=NULL;
    arc->nextArc = NULL;

    struct graphVertex *temp;
    struct graphVertex *fromVertex;
    struct graphVertex *toVertex;


    temp = head->firstVertex;


    while( (strcmp(temp->data,fromKey) != 0) && temp != NULL) // searching and finding fromVertex
    {
        temp = temp->nextVertex;

    }

    if(temp == NULL)
    {
        free(arc);
        return -1;
    }
    else
    {
        fromVertex = temp;
    }

    struct graphVertex *temp2 = head->firstVertex;


    while( (strcmp(temp2->data,toKey) != 0) && temp2 != NULL) // searching and finding toVertex
    {

        temp2 = temp2->nextVertex;

    }

    if(temp2 == NULL)
    {
        free(arc);

        return -2;
    }
    else
    {
        toVertex = temp2;
    }

    fromVertex->outdegree++;
    toVertex->degree+=weight;

    arc->destination = toVertex;

    arc->weight = weight;


    if(fromVertex->firstArc == NULL)
    {

        fromVertex->firstArc = arc;


    }

    else
    {

        struct graphArc *temparch = fromVertex->firstArc;

        while(temparch->nextArc != NULL)
        {
            temparch = temparch->nextArc;
        }

        temparch->nextArc = arc;
    }



    return 1;
}


struct graphHead * readMails(char *filePath,int number)
{
    struct mail* C = (struct mail*)malloc(number * sizeof(struct mail));
    struct graphHead* head = (struct graphHead*)malloc(sizeof(struct graphHead));
    head = createGraph();

    char *token;
    char name[50];
    FILE *inFile1;
    char fileName[50];
    char fileNumber[3] = "";
    int i,x;

    strcat(filePath,"\\");
    for(i=0; i < number; i++)  // O(N)
    {
        strcpy(fileName,filePath);
        itoa(i+1, fileNumber, 10);
        strcat(fileName,fileNumber);
        strcat(fileName,".txt");


        inFile1 = fopen(fileName,"r");


        if(i== 0)  // If the datapath is written incorrectly, the file will be empty so
            //I can find out that the data path name is misspelled.
        {
            if(inFile1 == NULL)
            {
                printf("There is no datapath !!!\n");
                exit(1);


            }

        }


        if(inFile1 == NULL)  // For more than 50 arguments
            printf("Much argument error!\n");

        fscanf(inFile1,"%d\n",&(C[i].Email_ID));  // Read integer value and store this value in my structure.


        fgets(name, 50, inFile1);
        token = strtok(name,":");
        token = strtok(NULL," ");       // I'm reading string values after ':'                                                                      //PROBLEM VAR

        strcpy(C[i].Sender,token);    // Then store in my structure.

        int size = strlen(C[i].Sender); //Total size of string
        C[i].Sender[size-1] = '\0';  // I am reading strings value with the one space, so here I am deleting the space.

        fgets(name, 50, inFile1);
        token = strtok(name,":");
        token = strtok(NULL," ");        // I'm reading string values after ':'

        strcpy(C[i].Receipt,token);       // Then store in my structure.

        strcpy(C[i].Receipt,token);  // Then store in my structure.
        size = strlen(C[i].Receipt); //Total size of string
        C[i].Receipt[size-1] = '\0'; // I am reading strings value with the one space, so here I am deleting the space.

        fscanf(inFile1,"%*s %d\n",&C[i].Day_of_the_month );  //skip string and read integer value, then store in my structure.


        int countforwords = 1; //I assumed content has least one word.
        char ch;


        while((ch = fgetc(inFile1)) != EOF)  // O(N)
        {

            if((ch == ' ')) // I read the spaces
            {
                countforwords++;
            }
        }


        C[i].countWords = countforwords;



     // IN HERE, I AM CALLING insertVertex function for Sender and Recepit
        insertVertex(head,C[i].Sender);
        insertVertex(head,C[i].Receipt);





        strcpy(fileName,"");
        fclose(inFile1);

    }
    printf("\n%d emails have been read successfully!\n\n ",number);





      //I added all the Receiver and Sender to my Graph above. Now,
      // I call the insertArc function and determine the edges of the vertexes.

    for(x =0; x< number ; x++) // number is the count of total mail.
    {
        insertArc(head,C[x].Sender,C[x].Receipt,C[x].countWords);
    }

    return (head);// I am returning head for main function.


}


void printGraph (struct graphHead *Furkan)

{

    struct graphVertex *vertex = Furkan->firstVertex;


    while(vertex != NULL)
    {
        printf("%s -> ",vertex->data);
        struct graphArc *arc2 = vertex->firstArc;

        while(arc2 != NULL)
        {

            printf("%s | %d",arc2->destination->data,arc2->weight);

            if(arc2->nextArc != NULL)
            {
                printf("-> ");
            }

            arc2 = arc2->nextArc;
        }

        printf("\n");
        vertex = vertex->nextVertex;

    }



    struct graphVertex *vertex2 = Furkan->firstVertex;
    int max = vertex2->outdegree;
    char name [50];

    while(vertex2 != NULL)
    {


        if(max < vertex2->outdegree)
        {
            max = vertex2->outdegree;  // Find the maximum value using "outdegree" information of vertices.


        }

        vertex2 = vertex2->nextVertex;

    }

    vertex2 = Furkan->firstVertex;

    while(vertex2 != NULL)
    {
        if(max == vertex2->outdegree)
        {
            printf("\nPeople with the maximum number of emails sent:\n ");
            printf("%s sent %d emails \n ",vertex2->data,max);

        }
        vertex2 = vertex2->nextVertex;
    }


    vertex2 = Furkan->firstVertex;
    int max2 = vertex2->degree;
    while(vertex2 != NULL)
    {


        if(max2 < vertex2->degree)
        {
            max2 = vertex2->degree;


        }

        vertex2 = vertex2->nextVertex;

    }

    vertex2 = Furkan->firstVertex;

    while(vertex2 != NULL)
    {
        if(max2 == vertex2->degree)
        {
            printf("\nPeople with the maximum number of words received:\n ");
            printf("%s received %d emails\n\n",vertex2->data,max2);

        }
        vertex2 = vertex2->nextVertex;
    }

}

struct graphVertex * checkFromVertexforPath(struct graphVertex *vertex, char sender [51])
{
    //This function: According to the entered Sender, it finds the vertex where the sender is located.

    while(vertex != NULL)
    {
        if(strcmp(vertex->data,sender) == 0)
        {
            return (vertex);
        }

        vertex = vertex->nextVertex;
    }



    return NULL;  // IF there is a not Vertex with this Sender name, it returns NULL.
}


int checkPath(struct graphVertex *vertex, char from [51] ,char reciver [51],int control)
{

if(control == 1)
     vertex = checkFromVertexforPath(vertex, from);  //finds the vertex where the sender is located.


if(vertex == NULL)  return 0 ;

struct graphArc *arc = vertex->firstArc;

    vertex->processed = 1;


    while(arc != NULL)
    {

/*Here I designed a recursion function.I set the vertex->processed value to 1 above,
so I prevented my function from entering the loop while traveling between destinations. If the processed value is 0,
my function calls itself again, if the arc->destination->data and receiver strings are the same, my function returns 1.
If the function returns 1, I understand that there is a connection, otherwise there is no path between Sender and Receiver.*/


        if(arc->destination->processed == 0)
        {
            if(checkPath(arc->destination,from,reciver,0))
                return 1;
        }

         if(strcmp(arc->destination->data,reciver) == 0)
          {    return 1;

          }

        arc = arc->nextArc;
    }

    return 0;

}

int main()
{

    int choose,number;

    char filePath[50];
    printf("Enter data path: ");
    scanf("%s",filePath);

    fflush(stdin);

    printf("How many files: ");
    scanf("%d",&number);

    struct graphHead *Furkan = readMails(filePath, number);


    int menu ;
    printf("Welcome :) \n\n");

    do
    {

        printf("Which option would you like to choose? \n");
        printf("1-)PrintGraph\n");
        printf("2-)CheckPath\n");
        printf("3-)Exit\n\n");
        printf("Select choice: ") ; scanf("%d",&menu);

        if(menu == 1)
            printGraph(Furkan);

        if(menu == 2)
        {

            struct graphVertex*tmp = Furkan->firstVertex;

  /*If I loop through the checkPath function more than once, the processed values may be 1 and errors may occur in my program.
  In order to avoid these errors, I set the processed value to 0 after each checkPath operation.*/

            while(tmp != NULL)
            {
                tmp->processed = 0;

                tmp = tmp->nextVertex;
            }

            printGraph(Furkan);
            char sender [50], reciver[50];

            printf("\nWhat is the first person's name ?\n");
            scanf("%s",sender);
            printf("\nWhat is the second person's name ?\n");
            scanf("%s",reciver);

            tmp = Furkan->firstVertex;

            int checkpath = checkPath(tmp,sender,reciver,1);


            if(checkpath == 1)
            {
                printf("There is a path in the graph from the first person to the second one.\n\n");
            }

            else
            {
                printf("\nThere is no path !!!\n");
            }


        }

        if(menu == 3)
        {
            printf("\n GOODBYE \n");
            exit(1);
        }

    }
    while(menu !=3);


    return 0;
}
