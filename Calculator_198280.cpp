//Author; Szymon Wisniewski
//Student ID: 198280
//Data engineering

#include <iostream>

//constants
#define BUFFOR 32
#define ASCII_ZERO 48
#define ASCII_NINE 57
#define DECIMAL 10

//types
#define NUMBER 0
#define LEFT 1
#define RIGHT 2
#define PLUS 3
#define MINUS 4
#define TIMES 5
#define DIVIDE 6
#define NEGATION 7
#define MIN 8
#define MAX 9
#define IF 10
#define SEPARATOR 11
#define ERROR 12

//priorities
#define BRACKETS_PRIORITY 4
#define PREFIX_PRIORITY 3
#define MULTIPLICATION_PRIORITY 2
#define ADDITION_PRIORITY 1
#define NO_PRIORITY 0

using namespace std;

//struct for returning string in functions
typedef struct {
    char string[BUFFOR];
}symbol;

//element of the list
typedef struct node {
    node* next; //adress of next element of the list
    int type;   //indicates if element is a number (type = 0), specific operand or specyfic function
    int value;  //stores value for number, stores number of arguments for functions
}node;


//data structue in form of a list
class List {
private:
    node* root;//stores first node adress
public:
    //constructor
    List();

    //ads an element to list directly after root
    void addFirst(node givenNode);

    //ads an element to list after all other elements
    void addLast(node givenNode);

    //disconnects the list form the root
    void cutRoot();

    //sets root
    void setRoot(node* newFirst);

    //returns and deletes the firt element of the list
    node pop();

    //returns adress of the first element
    node* getFirst();

    //returns adress of the last element
    node* getLast();

    //prints all values of all elements in the list
    void debug_printList();

    //prints all elements of the list according to the rules of the poject
    void printAll();

    //frees all allocated memory in the list
    void clearAll();

    //concets given list to the end of this list
    void connect(List* givenList);

    //destructor
    ~List();
};


//converts read data into a node, sets it's type and value
node convertInput(char input[BUFFOR]);

//converts given nodes type into string (return "NUM" for numbers)
symbol convertType(node* givenNode);

//converts given node into string for displaying (returns value for numbers)
symbol convertNode(node givenNode);

//returns priority for a given node
int getPriority(node* given);

//returns number of operands for a given operator or function
int getNumberOfoperants(node* given);

//checks if a node is storing a prefix function
bool isPrefix(node* given);

//converts read data and places it accordingly in the data structures
void convertAndManage(node* temp, char  read[32], List* result, List* operators);

//manages conversion of infix operators
void manageOperators(List* onp, List* operators, node operat);

//manages conversion of prefix functions
void managePrefixFunctions(node* head, List* onp);

//aplies an operation form given node to elements from stack
bool doCalcualtions(node* givenNode, List* stack);



//main

int main()
{
    int n_equations;
    char read[BUFFOR];
    node temp = {};
    List onp;
    List operators;

    cin >> n_equations;
    for (int equation = 0; equation < n_equations; equation++)
    {
        //reading and converting
        cin >> read;
        while (read[0] != '.')
        {

            convertAndManage(&temp, read, &onp, &operators);
            cin >> read;
        }
        while (operators.getFirst() != NULL)
        {
            onp.addLast(operators.pop());
        }

        //printing the postfix form
        onp.printAll();
        cout << endl;

        //doing calculations and displaying results
        List stack;
        bool executedCorrectly = 1;
        while (onp.getFirst() != NULL)
        {
            temp = onp.pop();
            if (!temp.type)
            {
                stack.addFirst(temp);
            }
            else
            {
                if (doCalcualtions(&temp, &stack)) //function returns 1 when it encounters division by zero
                {
                    cout << "ERROR" << endl;
                    executedCorrectly = 0;
                    stack.clearAll();
                    break;
                }
            }
        }
        if (executedCorrectly) cout << stack.getFirst()->value << endl;
        onp.clearAll();
        cout << endl;
    }
    return 0;
}



//List methods

List::List()
{
    root = NULL;
}

void List::addFirst(node givenNode) 
{
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL)
    {
        cout << "Failed to allocate memory";
        exit(1);
    }
    newNode->type = givenNode.type;
    newNode->value = givenNode.value;
    newNode->next = root;
    root = newNode;
}

void List::addLast(node givenNode)
{
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL)
    {
        cout << "Failed to allocate mempory";
        exit(1);
    }
    newNode->type = givenNode.type;
    newNode->value = givenNode.value;
    newNode->next = NULL;
    if (root == NULL)
    {
        root = newNode;
    }
    else
    {
        //getting last elements adress
        node* ptr = root;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = newNode;
    }
}

void List::cutRoot()
{
    root = NULL;
}

void List::setRoot(node* newFirst)
{
    root = newFirst;
}

node List::pop()
{
    if (root == NULL)
    {
        cout << "Stack is empty";
        exit(2);
    }
    node firstNode = *root;
    free(root);
    root = firstNode.next;
    return firstNode;
}

node* List::getFirst()
{
    return root;
}

node* List::getLast()
{
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        //getting last elements adress
        node* ptr = root;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        return ptr;
    }
}

void List::debug_printList()
{
    node* ptr = root;
    while (ptr != NULL)
    {
        cout << "node adress: " << ptr << " node value: " << ptr->value << " node type: " << convertType(ptr).string << " next adress: " << ptr->next << endl;
        ptr = ptr->next;
    }
    cout << "stack is empyt" << endl;
}

void List::printAll()
{
    node* ptr = root;
    while (ptr != NULL)
    {
        cout << convertNode(*ptr).string << " ";
        ptr = ptr->next;
    }
}

void List::connect(List* givenList)
{
    if (getFirst() != NULL)
    {
        getLast()->next = givenList->getFirst();
    }
    else
    {
        setRoot(givenList->getFirst());
    }
    givenList->cutRoot();
}

void List::clearAll()
{
    while (root != NULL)
    {
        this->pop();
    }
}

List::~List()
{
    clearAll();
}



//functions

node convertInput(char input[BUFFOR])
{
    node output;
    output.value = 0;
    output.type = 0;
    if (input[0] >= ASCII_ZERO && input[0] <= ASCII_NINE)   //checking if input is a number
    {
        output.type = NUMBER;
        int k = 0;
        while (input[k] != '\0')    //counting number of digits
        {
            k++;
        }
        for (int i = 0; i < k; i++) //converting string to integer
        {
            output.value += ((int)input[i] - ASCII_ZERO) * pow(DECIMAL, k - i - 1);
        }
        return output;
    }
    else    //assigning adequite value for symbols
    {
        if (input[0] == '(')output.type = LEFT;
        else if (input[0] == ')')output.type = RIGHT;
        else if (input[0] == '+')output.type = PLUS;
        else if (input[0] == '-')output.type = MINUS;
        else if (input[0] == '*')output.type = TIMES;
        else if (input[0] == '/')output.type = DIVIDE;
        else if (input[0] == ',')output.type = SEPARATOR;
        else if (input[0] == 'N')output.type = NEGATION;
        else if (input[0] == 'I' && input[1] == 'F')output.type = IF;
        else if (input[0] == 'M' && input[1] == 'I' && input[2] == 'N')output.type = MIN;
        else if (input[0] == 'M' && input[1] == 'A' && input[2] == 'X')output.type = MAX;
        else if (input[0] != '.')
        {
            cout << "unknown input: " << input;
            exit(3);
        }
        return output;
    }
}

symbol convertType(node* givenNode)
{
    symbol output;
    output.string[0] = '\0';
    if (givenNode->type == 0)
    {
        output.string[0] = 'N';
        output.string[1] = 'U';
        output.string[2] = 'M';
        output.string[3] = '\0';
        return output;
    }
    else if (givenNode->type == LEFT)output.string[0] = '(';
    else if (givenNode->type == RIGHT)output.string[0] = ')';
    else if (givenNode->type == PLUS)output.string[0] = '+';
    else if (givenNode->type == MINUS)output.string[0] = '-';
    else if (givenNode->type == TIMES)output.string[0] = '*';
    else if (givenNode->type == DIVIDE)output.string[0] = '/';
    else if (givenNode->type == NEGATION)output.string[0] = 'N';
    output.string[1] = '\0';
    int valueCopy = givenNode->value, i;
    List stack;
    node temp = {};
    if (givenNode->type == IF)
    {
        output.string[0] = 'I';
        output.string[1] = 'F';
        output.string[2] = '\0';
    }
    else if (givenNode->type == MIN)
    {
        output.string[0] = 'M';
        output.string[1] = 'I';
        output.string[2] = 'N';
    }
    else if (givenNode->type == MAX)
    {
        output.string[0] = 'M';
        output.string[1] = 'A';
        output.string[2] = 'X';
    }
    //for displaying the number of arguments of a function
    i = 3; //4th space in an array
    while (valueCopy > 0)  //converting integer into string (in reverse order)
    {
        temp.value = valueCopy % 10;
        stack.addFirst(temp);
        valueCopy /= 10;
    }
    while (stack.getFirst() != NULL)    //reversing to the proper order
    {
        output.string[i] = stack.pop().value + 48;
        i++;
    }
    output.string[i] = '\0';
    return output;
}

symbol convertNode(node givenNode)
{
    symbol output;
    if (givenNode.type)
    {
        output = convertType(&givenNode);   //name of type (symbol / function)
    }
    else
    {
        _itoa_s(givenNode.value, output.string, 10);    //value of a number in a string
    }
    return output;
}

int getPriority(node* given)
{
    if (given->type == LEFT || given->type == RIGHT) return BRACKETS_PRIORITY;
    if (given->type == MIN || given->type == MAX || given->type == IF || given->type == NEGATION) return PREFIX_PRIORITY;
    if (given->type == TIMES || given->type == DIVIDE) return MULTIPLICATION_PRIORITY;
    if (given->type == PLUS || given->type == MINUS) return ADDITION_PRIORITY;
    if (given->type == SEPARATOR || given->type == NUMBER) return NO_PRIORITY;
    cout << "Unknown type: " << given->type;
    exit(4);
}

int getNumberOfoperants(node* given)
{
    if (given->type == IF) return 3;
    if (given->type == TIMES || given->type == DIVIDE || given->type == PLUS || given->type == MINUS) return 2;
    if (given->type == NEGATION) return 1;
    if (given->type == MIN || given->type == MAX) return given->value;  //number of arguments of a function is stored in it's value variable
    cout << "Unknown type: " << given->type;
    exit(4);
}

bool isPrefix(node* given)
{
    if (getPriority(given) == PREFIX_PRIORITY)  //all prefix functions have same priority that no infix operand has
    {
        return 1;
    }
    return 0;
}

void convertAndManage(node* temp, char  read[32], List* result, List* operators)
{
    *temp = convertInput(read);
    if (temp->type == 0)
    {
        result->addLast(*temp);
    }
    else
    {
        if (isPrefix(temp)) //functions N, IF, MIN() and MAX() are prefix while operands + - * / are infix 
        {
            managePrefixFunctions(temp, result);    //prefix functions get special treatment 
        }
        else
        {
            manageOperators(result, operators, *temp);  //infix operands get normal treatment  
        }
    }
}

void manageOperators(List* onp, List* operators, node operat)
{
    if (operat.type == LEFT)
    {
        operators->addFirst(operat);
    }
    else if (operat.type == RIGHT)
    {

        while (operators->getFirst() != NULL)
        {
            node temp = operators->pop();
            if (temp.type == LEFT)break;
            onp->addLast(temp);
        }
    }
    else
    {
        int priority = getPriority(&operat);
        while (operators->getFirst() != NULL)
        {
            node temp;
            temp = operators->pop();
            if (getPriority(&temp) < priority || temp.type == LEFT)
            {
                operators->addFirst(temp);
                break;
            }
            onp->addLast(temp);
        }
        operators->addFirst(operat);
    }
}

void managePrefixFunctions(node* head, List* onp)
{
    List result, operators;
    char read[BUFFOR];
    node temp;
    cin >> read;


    temp = convertInput(read);
    if (read[0] == '(')
    {
        manageOperators(&result, &operators, temp);
        head->value = 1;    //accounts for number of arguments which can not be smaller than one    
        int level = 0;      //prevents premature breaking of the loop with expressions with parentheses inside it
        do {
            while (true)    
            {
                if (read[0] == '(')level++;
                if (read[0] == ')')level--;
                if (level <= 0)break;
                cin >> read;
                if (read[0] == ',')
                {
                    head->value++;  //counting number of arguments
                    while (operators.getFirst() != NULL && operators.getFirst()->type != LEFT)
                        result.addLast(operators.pop());
                    break;
                }
                convertAndManage(&temp, read, &result, &operators); //recurrent approach for nested functions
            }
        } while (level > 0);
    }
    else
    {
        convertAndManage(&temp, read, &result, &operators); //recurrent approach for multiple negations
    }

    //connecting the list from function to list higher up
    onp->connect(&result);
    onp->addLast(*head);
}

bool doCalcualtions(node* givenNode, List* stack)
{
    cout << convertType(givenNode).string << " ";
    stack->printAll();
    cout << endl;
    List operands;
    int a, b, c;
    for (int i = 0; i < getNumberOfoperants(givenNode); i++)    //getting adequate number of operands form stack
    {
        operands.addFirst(stack->pop());
    }
    if (givenNode->type == DIVIDE && operands.getLast()->value == 0)
    {
        return 1;   //when dividing by zero, prevents rest of the function from executing
    }
    node output = {};
    output.type = NUMBER;
    if (givenNode->type == PLUS)output.value = operands.pop().value + operands.pop().value;         
    else if (givenNode->type == MINUS)output.value = operands.pop().value - operands.pop().value;   
    else if (givenNode->type == TIMES)output.value = operands.pop().value * operands.pop().value;   
    else if (givenNode->type == DIVIDE)output.value = operands.pop().value / operands.pop().value;  
    else if (givenNode->type == NEGATION)output.value = -operands.pop().value;
    else if (givenNode->type == MIN)                                     
    {                                                                           
        output.value = operands.pop().value;
        while (operands.getFirst() != NULL)
        {
            a = operands.pop().value;
            if (a < output.value)
            {
                output.value = a;
            }
        }
    }
    else if (givenNode->type == MAX)
    {
        output.value = operands.pop().value;
        while (operands.getFirst() != NULL)
        {
            a = operands.pop().value;
            if (a > output.value)
            {
                output.value = a;
            }
        }
    }
    else if (givenNode->type == IF)
    {
        a = operands.pop().value;
        b = operands.pop().value;
        c = operands.pop().value;
        if (a > 0)
        {
            output.value = b;
        }
        else
        {
            output.value = c;
        }
    }
    stack->addFirst(output);
    return 0;   //returns 0 when not dividing by zero
}
