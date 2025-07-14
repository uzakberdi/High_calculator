#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_MEMORY_SIZE 5
#define PI 3.14159265358979323846
#define LOG_FILE "calculator.log"
#define MAX_LOG_LINE 256
#define PLOT_WIDTH 60
#define PLOT_HEIGHT 20
#define PLOT_CHAR '*'
#define AXIS_CHAR '+'
#define GRID_CHAR '.'


typedef struct {
    double data[MAX_MEMORY_SIZE];
    int size;
} Memory;

typedef struct {
    double *data;
    int size;
} Vector;


void clear_screen() {
    system("cls");
}

void show_result(const char *operation, double result) {
    printf("Operation: %s\n", operation);
    printf("Result: %.6lf\n", result);
}


void memory_init(Memory *mem) {
    mem->size = 0;
}

void memory_store(Memory *mem, double result) {
    if(mem->size >= MAX_MEMORY_SIZE) {
        for(int i = 1; i < MAX_MEMORY_SIZE; i++)
            mem->data[i-1] = mem->data[i];
        mem->data[MAX_MEMORY_SIZE-1] = result;
    } else {
        mem->data[mem->size++] = result;
    }
}

double memory_recall(Memory *mem, int index) {
    if(index < 0 || index >= mem->size) {
        printf("The specified memory cell is not available.\n");
        return 0.0;
    }
    return mem->data[index];
}

void memory_show(Memory *mem) {
    printf("Memory contents:\n");
    for(int i = 0; i < mem->size; i++) {
        printf("%d: %.6lf\n", i, mem->data[i]);
    }
}


double factorial(int n) {
    if(n < 0) {
        printf("Factorial of a negative number doesn't exist.\n");
        return 0.0;
    }
    if(n==0 || n==1) return 1.0;
    double result = 1.0;
    for(int i = 2; i <= n; i++) 
        result *= i;
    return result;
}


void vector_init(Vector *v, int size) {
    v->data = (double*)malloc(sizeof(double)*size);
    v->size = size;
}

void vector_free(Vector *v) {
    if(v->data) free(v->data);
    v->data = NULL;
    v->size = 0;
}

void vector_input(Vector *v) {
    printf("Enter %d numbers: ", v->size);
    for(int i = 0; i < v->size; i++)
        scanf("%lf", &v->data[i]);
    getchar();
}


double vector_sum(Vector *v) {
    double sum = 0.0;
    for(int i = 0; i < v->size; i++)
        sum += v->data[i];
    return sum;
}

double vector_mean(Vector *v) {
    return vector_sum(v) / v->size;
}

double vector_variance(Vector *v) {
    double mean = vector_mean(v);
    double sq_sum = 0.0;
    for(int i = 0; i < v->size; i++)
        sq_sum += pow(v->data[i] - mean, 2);
    return sq_sum / v->size;
}

double vector_stddev(Vector *v) {
    return sqrt(vector_variance(v));
}


double eval_expr(const char *expr, double x) {
    if(strstr(expr, "sin")) return sin(x);
    if(strstr(expr, "cos")) return cos(x);
    if(strstr(expr, "tan")) return tan(x);
    if(strstr(expr, "exp")) return exp(x);
    if(strstr(expr, "log")) return log(x);
    if(strstr(expr, "sqrt")) return sqrt(x);
    if(strstr(expr, "x^2")) return x * x;
    if(strstr(expr, "x^3")) return x * x * x;
    if(strstr(expr, "abs")) return fabs(x);
    if(strstr(expr, "1/x")) return 1.0 / x;
    return x;
}


double derivative(const char *expr, double x) {
    double h = 1e-7;
    double x1 = eval_expr(expr, x + h);
    double x2 = eval_expr(expr, x - h);
    return (x1 - x2) / (2*h);
}

double integral(const char *expr, double a, double b) {
    int n = 1000;
    double dx = (b - a) / n;
    double result = 0.0;
    for(int i = 0; i < n; i++) {
        double x = a + i * dx;
        result += eval_expr(expr, x) * dx;
    }
    return result;
}


void log_operation(const char* operation, double result) {
    time_t now;
    struct tm* timeinfo;
    char timestamp[20];
    
    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file != NULL) {
        fprintf(log_file, "[%s] %s: %f\n", timestamp, operation, result);
        fclose(log_file);
    }
}

void view_log(int last_n_lines) {
    FILE* log_file = fopen(LOG_FILE, "r");
    if (log_file==NULL) {
        printf("No log file found.\n");
        return;
    }

    char** lines = malloc(last_n_lines * sizeof(char*));
    int count = 0;
    char buffer[MAX_LOG_LINE];

    for(int i = 0; i < last_n_lines; i++) {
        lines[i] = NULL;
    }

    while (fgets(buffer, MAX_LOG_LINE, log_file)) {
        if(count < last_n_lines) {
            lines[count] = strdup(buffer);
        } else {
            free(lines[0]);
            for(int i = 0; i < last_n_lines-1; i++) {
                lines[i] = lines[i+1];
            }
            lines[last_n_lines-1] = strdup(buffer);
        }
        count++;
    }

    printf("Last %d operations:\n", last_n_lines);
    for(int i = 0; i < (count < last_n_lines ? count : last_n_lines); i++) {
        if(lines[i]) {
            printf("%s", lines[i]);
            free(lines[i]);
        }
    }
    free(lines);
    fclose(log_file);
}

void plot_function(const char *expr, double x_min, double x_max, double y_min, double y_max) {
    char plot[PLOT_HEIGHT][PLOT_WIDTH + 1];
    memset(plot, ' ', sizeof(plot));
    for(int i = 0; i < PLOT_HEIGHT; i++) {
        plot[i][PLOT_WIDTH] = '\0';
    }
    
    
    for(int i = 0; i < PLOT_HEIGHT; i++) {
        for(int j = 0; j < PLOT_WIDTH; j++) {
            if(i % 5 == 0 && j % 5 == 0) {
                plot[i][j] = GRID_CHAR;
            }
        }
    }
    
    
    int x_axis = (int)((0 - y_min) * PLOT_HEIGHT / (y_max - y_min));
    int y_axis = (int)((0 - x_min) * PLOT_WIDTH / (x_max - x_min));
    
    if(x_axis >= 0 && x_axis < PLOT_HEIGHT) {
        for(int j = 0; j < PLOT_WIDTH; j++) {
            plot[x_axis][j] = AXIS_CHAR;
        }
    }
    
    if(y_axis >= 0 && y_axis < PLOT_WIDTH) {
        for(int i = 0; i < PLOT_HEIGHT; i++) {
            plot[i][y_axis] = AXIS_CHAR;
        }
    }
    
    
    for(int j = 0; j < PLOT_WIDTH; j++) {
        double x = x_min + (x_max - x_min) * j / PLOT_WIDTH;
        double y = eval_expr(expr, x);
        
        if(y >= y_min && y <= y_max) {
            int i = (int)((y_max - y) * PLOT_HEIGHT / (y_max - y_min));
            if(i >= 0 && i < PLOT_HEIGHT) {
                plot[i][j] = PLOT_CHAR;
            }
        }
    }
    
    
    printf("\nPlot of %s from %.2f to %.2f\n\n", expr, x_min, x_max);
    printf("y-axis: %.2f to %.2f\n", y_max, y_min);
    
    for(int i = 0; i < PLOT_HEIGHT; i++) {
        if(i % 5 == 0) {
            double y = y_max - (y_max - y_min) * i / PLOT_HEIGHT;
            printf("%6.2f |", y);
        } else {
            printf("       |");
        }
        printf("%s\n", plot[i]);
    }
    
    printf("       ");
    for(int j = 0; j < PLOT_WIDTH + 1; j++) {
        printf("-");
    }
    printf("\n");
    
    printf("       ");
    for(int j = 0; j <= PLOT_WIDTH; j += PLOT_WIDTH/4) {
        double x = x_min + (x_max - x_min) * j / PLOT_WIDTH;
        printf("%-*.*f", PLOT_WIDTH/4, 2, x);
    }
    printf("\n");
}

void auto_plot_function(const char *expr) {
    
    double x_min = -5.0, x_max = 5.0;
    double y_min = -5.0, y_max = 5.0;
    
    
    int samples = 100;
    double dx = (x_max - x_min) / samples;
    
    y_min = y_max = eval_expr(expr, x_min);
    for(double x = x_min + dx; x <= x_max; x += dx) {
        double y = eval_expr(expr, x);
        if(y < y_min) y_min = y;
        if(y > y_max) y_max = y;
    }
    
    
    double y_margin = (y_max - y_min) * 0.1;
    y_min -= y_margin;
    y_max += y_margin;
    
    
    if(fabs(y_max - y_min) < 1e-10) {
        y_min = -1.0;
        y_max = 1.0;
    }
    
    plot_function(expr, x_min, x_max, y_min, y_max);
}

void print_menu() {
    printf("\nCalculator Menu:\n");
    printf("1. Basic arithmetic (+, -, *, /, ^)\n");
    printf("2. Trigonometric functions (sin, cos, tan)\n");
    printf("3. Derivative\n");
    printf("4. Integral\n");
    printf("5. Basic statistics\n");
    printf("6. Memory operations\n");
    printf("7. View log\n");
    printf("8. Clear screen\n");
    printf("9. Plot function\n");
    printf("q. Quit\n");
}

int main() {
    Memory mem;
    memory_init(&mem);
    printf("Calculator\n");
    print_menu();
    
    char input[64];
    while(1) {
        printf("\nEnter choice: ");
        fgets(input, sizeof(input), stdin);
        if(input[0]=='\n') continue;
        char choice = input[0];
        
        if(choice=='q') break;
        
        if(strchr("123456789", choice)==NULL) {
            printf("Invalid choice\n");
            continue;
        }

        double result = 0.0;
        char operation[MAX_LOG_LINE] = "";

        switch(choice) {
            case '1': {
                printf("Enter operation (+,-,*,/,^): ");
                char op;
                scanf(" %c", &op);
                printf("Enter two numbers: ");
                double a, b;
                scanf("%lf %lf", &a, &b);
                getchar();
                
                switch(op) {
                    case '+': result = a + b; sprintf(operation, "%.2f + %.2f", a, b); break;
                    case '-': result = a - b; sprintf(operation, "%.2f - %.2f", a, b); break;
                    case '*': result = a * b; sprintf(operation, "%.2f * %.2f", a, b); break;
                    case '/': 
                        if(b != 0) {
                            result = a / b;
                            sprintf(operation, "%.2f / %.2f", a, b);
                        } else {
                            printf("Error: Division by zero!\n");
                            continue;
                        }
                        break;
                    case '^': 
                        result = pow(a, b);
                        sprintf(operation, "%.2f ^ %.2f", a, b);
                        break;
                    default:
                        printf("Invalid operation\n");
                        continue;
                }
                break;
            }
            case '2': {
                printf("Enter function (sin/cos/tan): ");
                char func[10];
                scanf("%s", func);
                printf("Enter angle in radians: ");
                double angle;
                scanf("%lf", &angle);
                getchar();
                
                if(strcmp(func, "sin")==0) {
                    result = sin(angle);
                    sprintf(operation, "sin(%.2f)", angle);
                } else if(strcmp(func, "cos")==0) {
                    result = cos(angle);
                    sprintf(operation, "cos(%.2f)", angle);
                } else if(strcmp(func, "tan")==0) {
                    result = tan(angle);
                    sprintf(operation, "tan(%.2f)", angle);
                } else {
                    printf("Invalid function\n");
                    continue;
                }
                break;
            }
            case '3': {
                char func[64];
                printf("Enter function: ");
                scanf("%s", func);
                double x;
                printf("Point for derivative: ");
                scanf("%lf", &x);
                getchar();
                
                result = derivative(func, x);
                sprintf(operation, "d/dx[%s] at x=%.2f", func, x);
                break;
            }
            case '4': {
                char func[64];
                printf("Enter function: ");
                scanf("%s", func);
                double a, b;
                printf("Enter integration bounds (a b): ");
                scanf("%lf %lf", &a, &b);
                getchar();
                
                result = integral(func, a, b);
                sprintf(operation, "∫[%s]dx from %.2f to %.2f", func, a, b);
                break;
            }
            case '5': {
                printf("Enter number of values: ");
                int n;
                scanf("%d", &n);
                Vector v;
                vector_init(&v, n);
                vector_input(&v);
                getchar();
                
                printf("\n1. Mean\n2. Standard deviation\n");
                printf("Choose operation: ");
                int op;
                scanf("%d", &op);
                getchar();
                
                if(op==1) {
                    result = vector_mean(&v);
                    strcpy(operation, "Mean");
                } else if(op==2) {
                    result = vector_stddev(&v);
                    strcpy(operation, "Standard deviation");
                } else {
                    printf("Invalid choice\n");
                    vector_free(&v);
                    continue;
                }
                vector_free(&v);
                break;
            }
            case '6': {
                printf("1. Store\n2. Recall\n3. Show\n");
                printf("Choose operation: ");
                int op;
                scanf("%d", &op);
                getchar();
                
                if(op==1) {
                    printf("Enter value: ");
                    double val;
                    scanf("%lf", &val);
                    getchar();
                    memory_store(&mem, val);
                    printf("Value stored\n");
                    continue;
                } else if(op==2) {
                    printf("Enter index: ");
                    int idx;
                    scanf("%d", &idx);
                    getchar();
                    result = memory_recall(&mem, idx);
                    sprintf(operation, "Memory recall[%d]", idx);
                } else if(op==3) {
                    memory_show(&mem);
                    continue;
                } else {
                    printf("Invalid choice\n");
                    continue;
                }
                break;
            }
            case '7': {
                char func[64];
                printf("Enter function (e.g. sin, cos, x^2, exp): ");
                scanf("%s", func);
                getchar();
                
                printf("Plot options:\n");
                printf("1. Automatic range\n");
                printf("2. Custom range\n");
                printf("Choose option: ");
                
                int plot_option;
                scanf("%d", &plot_option);
                getchar();
                
                if(plot_option == 1) {
                    auto_plot_function(func);
                } else if(plot_option == 2) {
                    double x_min, x_max, y_min, y_max;
                    printf("Enter x_min x_max: ");
                    scanf("%lf %lf", &x_min, &x_max);
                    printf("Enter y_min y_max: ");
                    scanf("%lf %lf", &y_min, &y_max);
                    getchar();
                    
                    plot_function(func, x_min, x_max, y_min, y_max);
                }
                
                sprintf(operation, "Plot %s", func);
                continue;
            }
            
            case '8': {
                printf("Enter number of lines: ");
                int n;
                scanf("%d", &n);
                getchar();
                view_log(n);
                continue;
            }
            
            case '9':
                clear_screen();
                print_menu();
                continue;
        }

        show_result(operation, result);
        log_operation(operation, result);
        memory_store(&mem, result);
    }

    printf("Goodbye\n");
    return 0;
}
