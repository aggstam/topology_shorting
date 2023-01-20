// -------------------------------------------------------------------------------------
//
// This program implements a Topology sorting algorithm,
// based on Kahn's algorithm(https://en.wikipedia.org/wiki/Topological_sorting).
// In this version, algorithm calculations are performed in serial.
// Directed Acyclic Graph(DAG) is read from an input file created by RandomGraph generator 
// by S.Pettie and V.Ramachandran using the following arguments: 
// ./RandomGraph directed_grph_<N> <N> 2 1 <N/2>
// where N is the Graph nodes count we want to generate.
//
// Author: Angelos Stamatiou, March 2020
//
// -------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Queue node structure.
typedef struct node {
    int val;
    struct node* next;
} node;

node* head;                // Queue head node.
node* tail;                // Queue tail node.
FILE* fin;                 // Input file.
FILE* fout;                // Output file.
int nodes_count;           // Graph nodes count.
int** matrix;              // Graph nodes matrix.
int* dependencies_matrix;  // Graph nodes dependencies matrix.
int* topology_matrix;      // Graph nodes topology matrix.
int topology_matrix_index; // Graph nodes topology matrix index.

// This function inserts a given value at the end of the Queue.
// Inputs:
//      int val: The value to insert.
int push_value(int val) {
    node* new_node = (node*) malloc(sizeof(node));
    if (new_node == NULL) {
        return -1;
    }

    new_node->val = val;
    new_node->next = NULL;
    if (head == NULL) {
        head = new_node;
        tail = new_node;
        return 0;
    }

    tail->next = new_node;
    tail = new_node;

    return 0;
}

// This function returns the head value of the Queue.
// It will also move the head of the Queue to the next
// node, if it exists.
// Output:
//      retval --> Queue head value.
//      -1     --> Queue is empty.
int pop_value() {
    if (head == NULL) {
        return -1;
    }
    int retval = head->val;
    node *new_head = head->next;
    free(head);
    if (new_head == NULL) {
        head = NULL;
        tail = NULL;
    } else {
        head = new_head;    
    }

    return retval;
}

// This function initializes the Graph matrix and Dependencies matrix, by reading the input file.
// Output:
//      1 --> Initialized successfully.
//      0 --> Something went wrong.
int initialize() {
    int i,j,fscanf_result;
    double w;
    head = NULL;
    tail = NULL;
    matrix = (int**)malloc(sizeof(int*) * nodes_count + sizeof(int) * nodes_count * nodes_count);
    dependencies_matrix = (int*)malloc(sizeof(int*) * nodes_count);
    topology_matrix = (int*)malloc(sizeof(int*) * nodes_count);
    if ((matrix == NULL)  || (dependencies_matrix == NULL) || (topology_matrix == NULL)) {
        return 0;
    }

    int* ptr = (int*)(matrix + nodes_count);
    for(i = 0; i < nodes_count; i++) {
        matrix[i] = (ptr + nodes_count * i);
        dependencies_matrix[i] = 0;
        topology_matrix[i] = 0;    
        for (j = 0; j < nodes_count; j++) {
            matrix[i][j] = 0;            
        }
    }            
    fscanf_result = fscanf(fin, "%d", &i);
    while(fscanf_result != 1 || i != -1) {
        fscanf_result = fscanf(fin, "%d %lf \n", &j, &w);
        matrix[i][j] = 1;
        dependencies_matrix[j]++;    
        fscanf_result = fscanf(fin, "%d", &i);
    }

    return 1;
}

// Auxiliary function that displays a message in case of wrong input parameters.
// Inputs:
//      char* compiled_name: Programs compiled name.
void syntax_message(char* compiled_name) {
    printf("Correct syntax:\n");
    printf("%s <input-file> <output-file>\n", compiled_name);
    printf("where: \n");
    printf("<input-file> is the file containing a generated directed Graph by RandomGraph that the algorithm will use.\n");
    printf("<output-file> is the file Topology Matrix will be written.\n");
}

// This function checks run-time parameters validity and
// retrieves input and output file names.
// Inputs:
//      char** argv: The run-time parameters.
// Output:
//      1 --> Parameters read successfully.
//      0 --> Something went wrong.
int read_parameters(char** argv) {
    char* input_filename = argv[1];
    if (input_filename == NULL) {
        printf("Input file parameter missing.\n");
        syntax_message(argv[0]);
        return 0;
    }
            
    fin = fopen(input_filename, "r");
    if (fin == NULL) {
        printf("Cannot open input file %s.\n", input_filename);
        return 0;        
    }
    
    char* output_filename = argv[2];
    if (output_filename == NULL) {
        printf("Output file parameter missing.\n");
        syntax_message(argv[0]);
        return 0;
    }
            
    fout = fopen(output_filename, "w");
    if (fout == NULL) {
        printf("Cannot open output file %s.\n", output_filename);
        return 0;        
    }

    printf("Calculating Topology sorting of Graph.\n");
    printf("Graph will be retrieved from input file: %s\n", input_filename);
    printf("Topology Matrix will be written in output file: %s\n", output_filename);

    return 1;
}

// This function implements a Topology shorting algorithm,
// based on Kahn's algorithm.
void calculate_topology() {
    int i, current_node_index;
    
    // Push initial nodes(0 dependencies) to Queue.
    topology_matrix_index = 0;
    for (i = 0; i < nodes_count; i++) {
        if (dependencies_matrix[i] != 0) {
            continue;
        }
        if (push_value(i) == -1) {
            printf("Could not allocate memory.\n");
            exit(0);
        }
    }
    
    // While the Queue is not empty...
    current_node_index = pop_value(); // Retrieve Queue head
    while (current_node_index != -1) {
        for (i=0; i<nodes_count; i++) {
            if (matrix[current_node_index][i] != 1) {
                continue;
            }
            // Remove current node dependencies and check
            // if dependent nodes can be pushed to Queue.
            dependencies_matrix[i]--;
            if (dependencies_matrix[i] != 0) {
                continue;
            }
            // Push node to Queue.
            if (push_value(i) == -1) {
                printf("Could not allocate memory.\n");
                exit(0);
            }
        }
        // Insert current node index to Topology matrix.
        topology_matrix[topology_matrix_index] = current_node_index;
        topology_matrix_index++;
        // Retrieve next node(Queue head) to process.
        current_node_index = pop_value();
    }

    free(matrix);
    free(dependencies_matrix);
}

// This function writes the Topology matrix to the output file.
// First line contains the nodes count.
// Last line contains -1 as EOF char.
void write_topology_to_file() {
    fprintf(fout, "%d\n", nodes_count);
    for (int i = 0; i < nodes_count; i++) {
        fprintf(fout, "%d\n", topology_matrix[i]);            
    }
    fprintf(fout, "-1");
    free(topology_matrix);
}

int main(int argc, char** argv) {
    // Run-time parameters check.
    if (!read_parameters(argv)) {
        printf("Program terminates.\n");
        return -1;    
    }

    // Retrieve Graph nodes count.
    int fscanf_result = fscanf(fin, "%d \n", &nodes_count);
    if (fscanf_result == 1 && nodes_count == 0) {
        printf("File is empty.\n");
        fclose(fin);
        fclose(fout);
        printf("Program terminates.\n");
        return 0;
    }

    printf("Nodes count: %d\n", nodes_count);
    printf("Algorithm started, please wait...\n");
    // Initializes the Graph matrix and Dependencies matrix, by reading the input file.
    if (!initialize()) {
        printf("Failed to allocate memory during initialization.\n");
        return -1;
    }
    clock_t t1 = clock();
    // Calculate graphs Topology order.
    calculate_topology();
    clock_t t2 = clock();
    printf("Algorithm finished!\n");
    printf("Time spend: %f secs\n", ((float)t2 -t1) / CLOCKS_PER_SEC);
    printf("Writing Topology Matrix to output file.\n");
    // Write the Topology matrix to the output file.
    write_topology_to_file();

    fclose(fin);
    fclose(fout);
    printf("Program terminates.\n");

    return 0;
}
