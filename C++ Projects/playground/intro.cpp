#include <iostream>
#include <regex>
#include <string>
#include <random>
#include <fstream>

void printThis(char* a) {
    for (int i = 0; a[i] != '\0'; i++) {
        std::cout << a[i] << std::ends;    
    };
}

class Line {
    public:
        void setLength(double len);
        double getLength(void);
        //overloaded constructor
        //this is the paramerized constructor, user has to provide the arguments for this to be called
        explicit Line(double len) {
            length = len;
            std::cout << "Parameterized overloaded constructor called" << std::endl;
        }; 
        Line() : length(0) {
            std::cout << "Default constructor called" << std::endl;
        }; //non-parameter constructor
        ~Line(void) {
            std::cout << "Line Object is being deleted" << std::endl;
        }; //destructor
        
    private:
        double length;
};

//this is defining a member function outside of the class
double Line::getLength(void) {
    return length;
}
void Line::setLength(double len) {
    length = len;
}


class Shape { //base class
    public:
        void setWidth(int w) {
            width = w;
        }
        void setHeight(int h) {
            height = h;
        }

    protected:
        int width;
        int height;
        static int objectCount;
};

class PaintCost {
    public:
        int getCost(int area) {
            return area * 70;
        }
};

class Rectangle: public Shape, public PaintCost { //derived class
    public:
        int getArea() {
            return (width * height);
        }
};

/*
Receives methods from Shape, PaintCost, and Rectangle.
*/
class Rhombus: public Rectangle { //multilevel inheritance
    public:
        void type() {
            std::cout << "A Rhombus might be a rectangle?" << std::endl;
        }
        //'this' is an implicit parameter for all member functions, refers to the current instance of the object
        //it is a pointer that all objects have to their own memory address
        int compare(Rhombus rhombus) {
            return this->getArea() > rhombus.getArea();
        }
};

//Scope resolution
class Base1 {
    public:
        void show() {std::cout << "Base1 show" << std::endl;}
        ~Base1(void) {
            std::cout << "Base1 Object being deleted" << std::endl;
        }
};
class Base2 {
    public:
        void show() {std::cout << "Base2 show" << std::endl;}
        ~Base2(void) {
            std::cout << "Base2 Object being deleted" << std::endl;
        }
};
class Derived : public Base1, public Base2 {
    public:
        void show() {
            Base1::show();
            Base2::show();
        }

        ~Derived(void) {
            std::cout << "Derived Object being deleted" << std::endl;
        }
};

//exploring the diamond problem
//removing the 'virtual' will cause ambiguity
//virtual ensures only one instance of the base class exists at runtime
class Derived1 : virtual public Base1 {};
class Derived2 : virtual public Base1 {};

class Final : public Derived1, public Derived2 {};

class Book {
    private:
        std::string title;
        std::string author;
    public:
        static int totalBooks;

    Book(std::string bookTitle, std::string bookAuthor) {
        title = bookTitle;
        author = bookAuthor;
        totalBooks++; //increment total books for each book object created
    }
    //static method to display the total num of books
    static void displayTotalBooks() {
        std::cout << "Total number of books in the library: " << totalBooks << std::endl;
    }
    void displayBookInfo() {
        std::cout << "Book Title: " << title << ", Author: " << author << std::endl;
    }
};

//initialize static data member outside of the class
int Book::totalBooks = 0;


class Coordinates {
    private:
        int latitude;
        int longitude;
    
    public:
        Coordinates(int lat = 0, int lon = 0) {
            this->latitude = lat;
            this->longitude = lon;
        }

        Coordinates& setLatitude(int lat) {
            latitude = lat;
            return *this;
        }

        //figure out what the ampersand is doing
        Coordinates& setLongitude(int lon) {
            longitude = lon;
            return *this;//dereferencing to get what this points to
        }

        void display() const {
            std::cout << "Latitude = " << latitude << ", Longitude = " << longitude << std::endl;
        }
};

// class Parser {
//     public:
//     void extractor(std::string &data, std::string &attr, std::string marker) {
//         std::string del = " ";
//         std::string trash;
//         std::string returnData = data;
//         std::string returnAttr = attr;
//         auto pos = returnData.find(del);
//         while(pos != std::string::npos) {
//             if(returnData.substr(0,pos) != marker) {
//                 returnAttr.append(returnData.substr(0, pos) + " ");
//                 returnData.erase(0, pos + del.length());
//                 pos = returnData.find(del);
//             }
//             else {
//                 trash.append(returnData.substr(0, pos));
//                 returnData.erase(0, pos + del.length());
//                 pos = returnData.find(del);
//             }
//         }
//         returnAttr.append(returnData);
//         attr = returnAttr;
//         std::string delimiter = " ";
//     }

//     Monster readMonsterFromFile(std::string fileName) {
//         std::string del = " ";
//         std::string extract;
//         std::string NAME, SYMB, COLOR, DESC, SPEED, DAM, HP, RRTY, ABIL;
//         std::ifstream file(fileName);
//         if(!file.is_open()) {
//             std::cerr << "Error in opening " << fileName << std::endl;
//         }
//         std::string data = "";
//         bool correctFormat = false;
//         while(std::getline(file, data)) {
//             if(!correctFormat) {
//                 if(data == "RLG327 MONSTER DESCRIPTION 1" || "rlg327 monster description 1") {
//                     correctFormat = true;
//                 }
//                 else {
//                     std::cout << "File was not to specifications" << std::endl;
//                     break;
//                 }
//             }
//             if(data == "BEGIN MONSTER") {
//                 std::getline(file, data);
//                 //NAME
//                 extractor(data, NAME, "NAME");
//                 std::getline(file, data);

//                 //SYMB
//                 extractor(data, SYMB, "SYMB");
//                 std::getline(file, data);

//                 //COLOR
//                 extractor(data, COLOR, "COLOR");
//                 std::getline(file, data);

//                 //DESC
//                 std::getline(file, data);
//                 while(data != ".") {
//                     DESC.append(data + " ");
//                     std::getline(file, data);
//                 }
//                 //std::cout << "extracted description: " << DESC << std::endl;
//                 std::getline(file, data);

//                 //SPEED
//                 extractor(data, SPEED, "SPEED");
//                 std::getline(file, data);

//                 //DAM
//                 extractor(data, DAM, "DAM");
//                 std::getline(file, data);

//                 //HP
//                 extractor(data, HP, "HP");
//                 std::getline(file, data);

//                 //RRTY
//                 extractor(data, RRTY, "RRTY");
//                 std::getline(file, data);

//                 //ABIL
//                 extractor(data, ABIL, "ABIL");
//                 std::getline(file, data);

//             }
//             //std::cout << data << std::endl;
//         }
//         //std::cout << "Reached end of file, exiting..." << std::endl;
//         file.close();
//         Monster newMonster(NAME, DESC, COLOR, SPEED, ABIL, HP, DAM, SYMB, RRTY);
//         return newMonster;
//     }
// };

class Monster {};














int main() {
    std::cout << "Hello World!" << std::endl;
    bool flag = false;
    if(!flag) std::cout << "flag was false" << std::endl;
    char lines[] = {'t', 'h', 'i', 's', ' ', 'w', 'o', 'r', 'k', 'e', 'd', '\n','\0'};
    printThis(lines);
    Line line1;
    Line line(10.0); //parameterized overloaded constr
    //getting initially set length
    std::cout << "Length of line: " << line.getLength() << std::endl;
    std::cout << "Length of non-param line: " << line1.getLength() << std::endl;
    //setting line length
    line1.setLength(6.0);
    std::cout << "Length of line1 now: " << line1.getLength() << std::endl;
    Rectangle Rect;
    Rect.setWidth(5);
    Rect.setHeight(7);

    std::cout << "Total area: " << Rect.getArea() << std::endl;
    std::cout << "Total paint cost: $" << Rect.getCost(Rect.getArea()) << std::endl;

    Derived derived;
    derived.show();

    Final obj;
    obj.show(); //this causes ambiguity if inherited classes do not use virtual
    
    Rhombus rhombus;
    rhombus.setHeight(10);
    rhombus.setWidth(10);
    std::cout << "Total paint cost: $" << rhombus.getCost(rhombus.getArea()) << std::endl;
    rhombus.type();

    Book book1("The Catcher in the Rye", "J.D. Salinger");
    Book book2("To Kill a Mockingbird", "Harper Lee");
    Book book3("1984", "George Orwell");

    //display total num of books
    Book::displayTotalBooks();

    //display details of books
    book1.displayBookInfo();
    book2.displayBookInfo();
    book3.displayBookInfo();

    Book book4("Pride and Prejudice", "Jane Austen");
    book4.displayBookInfo();

    Book::displayTotalBooks(); //calling static method

    Coordinates location(15,30);
    location.setLatitude(40).setLongitude(70); //chained function calls modifying the same object

    location.display();

    //delimiter
    std::string del = " ";
    std::string str = "NAME Barry B Benson";
    std::string NAME;
    std::string test;
    //find first occurance of the delimiter
    auto pos = str.find(del);
    //while there are still delimiters in the string
    // while(pos != std::string::npos) {
    //     //extracting substring up to the delimiter
    //     std::cout << "\"" << str.substr(0, pos) << "\"" << " ";
    //     //erase extracted part from the original string
    //     str.erase(0, pos + del.length());
    //     //find next occurance of the delimiter
    //     pos = str.find(del);
    // }
    while(pos != std::string::npos) {
        if(str.substr(0,pos) != "NAME") {
            NAME.append(str.substr(0, pos) + " ");
            str.erase(0, pos + del.length());
            pos = str.find(del);
        }
        else {
            test.append(str.substr(0, pos));
            str.erase(0, pos + del.length());
            pos = str.find(del);
        }
        //i++;)
    }
    NAME.append(str);
    //output last substring (after last delimiter)
    //std::cout << "\"" << str << "\"" << " " << std::endl;
    std::cout << NAME << std::endl;
    std::cout << "Extracted portions: " << test << std::endl;

    std::string speedDice = "12+1d6";
    std::string constantStr, diceRollStr1, diceRollStr2 = "";
    int constant, numDice, numFaces = 0;
    int diceRoll = 0;
    std::string delim1 = "+";
    std::string delim2 = "d";
    //split string
    auto pos1 = speedDice.find(delim1);
    constantStr = speedDice.substr(0, pos1);
    speedDice.erase(0, pos1 + delim1.length());
    pos1 = speedDice.find(delim2);
    diceRollStr1 = speedDice.substr(0, pos1);
    speedDice.erase(0, pos1 + delim2.length());
    diceRollStr2 = speedDice;
    //get integers
    constant = std::stoi(constantStr);
    numDice = std::stoi(diceRollStr1);
    numFaces = std::stoi(diceRollStr2); //max roll of the die
    int min = 1; //minimum roll of a die
    
    //generate bounded random
    std::random_device roll;
    std::mt19937 gen(roll());
    std::uniform_int_distribution<> distrib(min, numFaces); //creates norm_distr generator object
    std::cout << "Starting diceRoll: " << diceRoll << std::endl;
    for(int i = 0; i != numDice; i++) {
        diceRoll += distrib(gen);
        std::cout << "diceRoll: " << diceRoll << std::endl;
    }
    diceRoll += constant;
    std::cout << "Final diceRoll: " << diceRoll << std::endl;
    std::string testing = "p";
    char newTest = testing[0];
    std::cout << newTest << std::endl;
    return 0;

}