#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <cstring>
#include <map>
#include <stack>
#include <cctype>

// Constants for visualization
const int DISPLAY_WIDTH = 50;    // Display width of calculator
const int PLOT_WIDTH = 60;       // Plot width
const int PLOT_HEIGHT = 20;      // Plot height
const char PLOT_POINT = '*';     // Plot point character
const char PLOT_LINE = '-';      // Line character
const char PLOT_VERTICAL = '|';  // Vertical line character
const char PLOT_AXIS = '+';      // Axis character
const int MAX_MEMORY_SIZE = 5;    // Maximum number of stored results
const char PLOT_GRID = '.';       // Grid character for plot

class Display {
public:
    static void showTitle() {
        std::cout << "\n+"; 
        std::cout << std::string(DISPLAY_WIDTH-2, '=');
        std::cout << "+\n";
        std::cout << "| " << std::setw(DISPLAY_WIDTH-3) << std::left 
                  << "Scientific Calculator v2.0" << "|\n";
        std::cout << "+";
        std::cout << std::string(DISPLAY_WIDTH-2, '=');
        std::cout << "+\n\n";
    }

    static void showResult(const std::string& operation, double result) {
        std::cout << "\n+";
        std::cout << std::string(DISPLAY_WIDTH-2, '-');
        std::cout << "+\n";
        std::cout << "| Operation: " << std::setw(DISPLAY_WIDTH-12) << std::left 
                  << operation << "|\n";
        std::cout << "| Result: " << std::fixed << std::setprecision(6) 
                  << std::setw(DISPLAY_WIDTH-13) << std::left << result << "|\n";
        std::cout << "+";
        std::cout << std::string(DISPLAY_WIDTH-2, '-');
        std::cout << "+\n\n";
    }

    static void plotFunction(const std::vector<std::pair<double, double>>& points,
                            double minX, double maxX, double minY, double maxY) {
        std::vector<std::string> canvas(PLOT_HEIGHT, std::string(PLOT_WIDTH, PLOT_GRID));
        
        // Drawing axes
        int zeroY = PLOT_HEIGHT/2;
        int zeroX = PLOT_WIDTH/2;
        
        // Horizontal axis
        for (int x = 0; x < PLOT_WIDTH; x++) {
            canvas[zeroY][x] = PLOT_LINE;
        }
        
        // Vertical axis
        for (int y = 0; y < PLOT_HEIGHT; y++) {
            canvas[y][zeroX] = PLOT_VERTICAL;
        }

        // Mark intersection point
        canvas[zeroY][zeroX] = PLOT_AXIS;
        
        // Drawing points and coordinates
        for (const auto& point : points) {
            int x = static_cast<int>((point.first - minX) / (maxX - minX) * (PLOT_WIDTH - 1));
            int y = PLOT_HEIGHT - 1 - static_cast<int>((point.second - minY) / (maxY - minY) * (PLOT_HEIGHT - 1));
            
            if (x >= 0 && x < PLOT_WIDTH && y >= 0 && y < PLOT_HEIGHT) {
                canvas[y][x] = PLOT_POINT;
            }
        }

        // Plot output with axis labels
        std::cout << "\n  Function plot:\n\n";
        
        // Print Y-axis values
        for (int y = 0; y < PLOT_HEIGHT; y++) {
            if (y == 0)
                std::cout << std::setw(6) << std::fixed << std::setprecision(1) << maxY << " ";
            else if (y == PLOT_HEIGHT - 1)
                std::cout << std::setw(6) << minY << " ";
            else if (y == zeroY)
                std::cout << std::setw(6) << "0.0" << " ";
            else
                std::cout << std::setw(7) << " ";
                
            std::cout << canvas[y] << "\n";
        }
        
        // Print X-axis values
        std::cout << std::setw(7) << " ";
        std::cout << std::fixed << std::setprecision(1) 
                  << minX << std::string((PLOT_WIDTH/2)-6, ' ') << "0.0"
                  << std::string((PLOT_WIDTH/2)-6, ' ') << maxX << "\n\n";
    }
};

// Add this class before the Calculator class
class Memory {
private:
    static std::vector<double> stored_results;
public:
    static void store(double result) {
        if (stored_results.size() >= MAX_MEMORY_SIZE) {
            stored_results.erase(stored_results.begin());
        }
        stored_results.push_back(result);
    }
    
    static double recall(int index) {
        if (index < 0 || index >= stored_results.size()) {
            throw std::runtime_error("Invalid memory index");
        }
        return stored_results[index];
    }
    
    static void showMemory() {
        std::cout << "\nStored results:\n";
        for (size_t i = 0; i < stored_results.size(); ++i) {
            std::cout << i << ": " << stored_results[i] << "\n";
        }
    }
};

std::vector<double> Memory::stored_results;

class ExpressionEvaluator {
private:
    static bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

    static int getPrecedence(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    static double applyOperator(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw std::runtime_error("Division by zero");
                return a / b;
            case '^': return std::pow(a, b);
            default: throw std::runtime_error("Unknown operator");
        }
    }

    static double evaluateFunction(const std::string& name, double x) {
        if (name == "sin") return std::sin(x);
        if (name == "cos") return std::cos(x);
        if (name == "tan") return std::tan(x);
        if (name == "exp") return std::exp(x);
        if (name == "log") return std::log(x);
        if (name == "sqrt") return std::sqrt(x);
        throw std::runtime_error("Unknown function: " + name);
    }

public:
    static double evaluate(const std::string& expression, double x) {
        std::stack<double> values;
        std::stack<char> ops;
        std::string token;
        bool expectOperator = false;

        for (size_t i = 0; i < expression.length(); ++i) {
            char c = expression[i];

            if (std::isspace(c)) continue;

            if (std::isalpha(c)) {
                token = "";
                while (i < expression.length() && std::isalpha(expression[i])) {
                    token += expression[i++];
                }
                i--;

                if (token == "x") {
                    values.push(x);
                    expectOperator = true;
                } else {
                    if (i + 1 < expression.length() && expression[i + 1] == '(') {
                        ops.push('f');
                        i++;
                    } else {
                        throw std::runtime_error("Expected opening parenthesis after function");
                    }
                }
                continue;
            }

            if (std::isdigit(c) || c == '.') {
                token = "";
                while (i < expression.length() && (std::isdigit(expression[i]) || expression[i] == '.')) {
                    token += expression[i++];
                }
                i--;
                values.push(std::stod(token));
                expectOperator = true;
                continue;
            }

            if (c == '(') {
                ops.push(c);
                expectOperator = false;
            }
            else if (c == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    char op = ops.top();
                    ops.pop();

                    if (op == 'f') {
                        double val = values.top();
                        values.pop();
                        values.push(evaluateFunction(token, val));
                    } else {
                        double b = values.top(); values.pop();
                        double a = values.top(); values.pop();
                        values.push(applyOperator(a, b, op));
                    }
                }
                if (!ops.empty()) ops.pop();
                expectOperator = true;
            }
            else if (isOperator(c)) {
                if (!expectOperator && c == '-') {
                    values.push(0);
                }
                while (!ops.empty() && ops.top() != '(' && getPrecedence(ops.top()) >= getPrecedence(c)) {
                    char op = ops.top();
                    ops.pop();
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    values.push(applyOperator(a, b, op));
                }
                ops.push(c);
                expectOperator = false;
            }
        }

        while (!ops.empty()) {
            char op = ops.top();
            ops.pop();
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            values.push(applyOperator(a, b, op));
        }

        return values.top();
    }
};

// Class for mathematical operations
class Calculator {
private:
    static std::string userFunction;

public:
    static void setUserFunction(const std::string& func) {
        userFunction = func;
    }

    static double evaluateFunction(double x) {
        return ExpressionEvaluator::evaluate(userFunction, x);
    }

    static double calculate(char op, const std::vector<double>& numbers) {
        switch (op) {
            case '+': return std::accumulate(numbers.begin(), numbers.end(), 0.0);
            case '-': {
                double result = numbers[0];
                for (size_t i = 1; i < numbers.size(); ++i) result -= numbers[i];
                return result;
            }
            case '*': return std::accumulate(numbers.begin(), numbers.end(), 1.0, std::multiplies<double>());
            case '/': {
                double result = numbers[0];
                for (size_t i = 1; i < numbers.size(); ++i) {
                    if (numbers[i] == 0) throw std::runtime_error("Division by zero");
                    result /= numbers[i];
                }
                return result;
            }
            case '^': return std::pow(numbers[0], numbers[1]);
            default: throw std::runtime_error("Unknown operation");
        }
    }

    static double derivative(double x) {
        const double h = 1e-7;  // Smaller h for better accuracy
        const double x1 = evaluateFunction(x + h);
        const double x2 = evaluateFunction(x - h);
        const double x3 = evaluateFunction(x + 2*h);
        const double x4 = evaluateFunction(x - 2*h);
        return (-x3 + 8*x1 - 8*x2 + x4)/(12*h);  // 4th order accurate formula
    }

    static double integral(double a, double b) {
        const int n = 1000;
        const double dx = (b - a) / n;
        double result = 0.0;
        
        for (int i = 0; i < n; i++) {
            double x = a + i * dx;
            result += evaluateFunction(x) * dx;
        }
        
        return result;
    }

    static std::vector<std::pair<double, double>> generatePoints(double start, double end, int points) {
        std::vector<std::pair<double, double>> result;
        double step = (end - start) / (points - 1);
        
        for (int i = 0; i < points; i++) {
            double x = start + i * step;
            try {
                double y = evaluateFunction(x);
                result.push_back({x, y});
            } catch (...) {
                result.push_back({x, 0});
            }
        }
        
        return result;
    }

    static std::vector<double> basicStats(const std::vector<double>& numbers) {
        if (numbers.empty()) {
            throw std::runtime_error("No numbers provided");
        }
        if (numbers.size() == 1) {
            return {numbers[0], 0.0, numbers[0], numbers[0]};
        }
        
        double sum = std::accumulate(numbers.begin(), numbers.end(), 0.0);
        double mean = sum / numbers.size();
        
        double sqSum = std::accumulate(numbers.begin(), numbers.end(), 0.0,
            [mean](double acc, double x) { return acc + (x - mean) * (x - mean); });
        double stddev = std::sqrt(sqSum / numbers.size());
        
        double min = numbers[0], max = numbers[0];
        for (const auto& num : numbers) {
            if (num < min) min = num;
            if (num > max) max = num;
        }
        
        return {mean, stddev, min, max};
    }
};

std::string Calculator::userFunction = "sin(x)";

int main() {
    std::cout << std::fixed << std::setprecision(6);
    Display::showTitle();
    
    std::cout << "Welcome to the Scientific Calculator! 🧮\n\n";
    std::cout << "Here's what we can do:\n";
    std::cout << "1. Basic operations:\n";
    std::cout << "   +  : Add numbers together\n";
    std::cout << "   -  : Subtract numbers\n";
    std::cout << "   *  : Multiply numbers\n";
    std::cout << "   /  : Divide numbers\n";
    std::cout << "   ^  : Raise to a power\n\n";
    std::cout << "2. Advanced math:\n";
    std::cout << "   d  : Find the derivative\n";
    std::cout << "   i  : Calculate the integral\n";
    std::cout << "   p  : Draw a function plot\n\n";
    std::cout << "3. Statistics:\n";
    std::cout << "   s  : Calculate statistics (mean, stddev, min, max)\n\n";
    std::cout << "Available math functions:\n";
    std::cout << "   sin, cos, tan, exp, log, sqrt\n";
    std::cout << "Note: Use 'x' as the variable when writing functions\n\n";
    std::cout << "   m  : Show memory\n";
    std::cout << "   r  : Recall from memory\n";  // Add this line

    while (true) {
        try {
            std::cout << "What would like to calculate? (press q to exit): ";
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) continue;
            char op = input[0];

            if (op == 'q') break;

            if (strchr("+-*/^dipsrm", op) == nullptr) {  // Add 'r' here
                throw std::runtime_error("Invalid operation");
            }

            if (op == 'd' || op == 'i' || op == 'p') {
                std::cout << "Enter function (e.g., sin(x) or x^2): ";
                std::string func;
                std::getline(std::cin, func);
                Calculator::setUserFunction(func);

                if (op == 'd') {
                    double x;
                    std::cout << "At which point should we calculate the derivative? ";
                    if (!(std::cin >> x)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::runtime_error("Hmm, that doesn't look like a valid number. Let's try again!");
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    auto points = Calculator::generatePoints(x - 2, x + 2, PLOT_WIDTH + 1);
                    double result = Calculator::derivative(x);
                    Display::showResult("Function derivative", result);
                    Display::plotFunction(points, x-2, x+2, -1.5, 1.5);
                }
                else if (op == 'i') {
                    double a, b;
                    std::cout << "Let's integrate! Enter the start and end points: ";
                    if (!(std::cin >> a >> b)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::runtime_error("Oops! Need two valid numbers for integration. Let's try again!");
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "\nCalculating integral, please wait...\n";
                    auto points = Calculator::generatePoints(a, b, PLOT_WIDTH + 1);
                    double result = Calculator::integral(a, b);
                    Display::showResult("Function integral", result);
                    Display::plotFunction(points, a, b, -1.5, 1.5);
                }
                else if (op == 'p') {
                    double start, end;
                    std::cout << "Let's draw a graph! Enter the start and end points: ";
                    if (!(std::cin >> start >> end)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::runtime_error("Need two numbers to create the plot. Let's try again!");
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "\nGenerating plot...\n";
                    auto points = Calculator::generatePoints(start, end, PLOT_WIDTH + 1);
                    Display::plotFunction(points, start, end, -1.5, 1.5);
                }
            }
            else if (op == 's') {
                std::cout << "Enter numbers for statistics (separated by spaces): ";
                std::string input;
                std::getline(std::cin, input);
                std::stringstream ss(input);
                std::vector<double> numbers;
                double num;
                while (ss >> num) {
                    numbers.push_back(num);
                }
                auto stats = Calculator::basicStats(numbers);
                std::cout << "\nStatistics:\n";
                std::cout << "Mean: " << stats[0] << "\n";
                std::cout << "Standard deviation: " << stats[1] << "\n";
                std::cout << "Min: " << stats[2] << "\n";
                std::cout << "Max: " << stats[3] << "\n";
                continue;
            }
            else if (op == 'm') {
                Memory::showMemory();
                continue;
            }
            else if (op == 'r') {  // Add this block
                std::cout << "Enter memory index to recall: ";
                int index;
                if (!(std::cin >> index)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::runtime_error("Invalid index");
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                double result = Memory::recall(index);
                Display::showResult("Memory recall", result);
                continue;
            }
            else {
                std::cout << "Enter the numbers to calculate (separated by spaces): ";
                std::string input;
                std::getline(std::cin, input);
                std::stringstream ss(input);
                std::vector<double> numbers;
                double num;
                
                bool hasValidInput = false;
                while (ss >> num) {
                    numbers.push_back(num);
                    hasValidInput = true;
                }

                if (!hasValidInput) {
                    throw std::runtime_error("I need some valid numbers to work with! Let's try again.");
                }

                double result = Calculator::calculate(op, numbers);
                Display::showResult("Basic operation", result);
                Memory::store(result);
            }
        }
        catch (const std::exception& e) {
            std::cout << "\n " << e.what() << "\n\n";
        }
        std::cout << "----------------------------------------\n";
    }

    std::cout << "\nBye!\n";
    return 0;
}