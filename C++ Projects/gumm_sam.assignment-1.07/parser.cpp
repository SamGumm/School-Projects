#include <iostream> //used for cout
#include <fstream> //used for opening the file
#include <string> //used for string manipulation
#include <vector> //used for vectors
#include <random> //used for dice rolls

class Monster {
    public:
        std::string NAME; //Name to describe monster terminated by newline
        std::string DESC; //Textual description of the monster MAX 77 bytes long, terminated by newline (can be 78th byte)
        /*
        RED
        GREEN
        BLUE
        CYAN
        YELLOW
        MAGENTA
        WHITE
        BLACK
        */
        std::vector<std::string> COLOR{}; //A list of space-delineated color name keywords followed by a newline
        int SPEED; //Numerical value determined by dice rolls followed by newline
        /*
        TELE: 
        TUNNEL: 
        ERRATIC:
        PASS:
        PICKUP: 
        DESTROY:
        BOSS:
        */
        std::string ABIL; //given abilities of the monster, list above, terminated by newline
        int HP; //numerical value for health of the monster, terminated by newline 
        int DAM; //numerical value for damage capability of the monster, terminated by newline
        char SYMB; //a single character giving the in-game graphical representation of the monster
        int RRTY; //integer between 1-100, inclusive, giving probability of this monster appearing
        std::string speedDice, hpDice, damDice; //used for printing

        void attack() {
            //initiate an attack
        };

        void move() {
            //move to a new cell
        };

        int rollDice(std::string rollDice) { //sooo needlessly complicated T_T
            std::string constantStr, diceRollStr1, diceRollStr2 = "";
            int constant, numDice, numFaces = 0;
            int diceRoll = 0;
            std::string delim1 = "+";
            std::string delim2 = "d";
            //split string
            auto pos1 = rollDice.find(delim1);
            constantStr = rollDice.substr(0, pos1);
            rollDice.erase(0, pos1 + delim1.length());
            pos1 = rollDice.find(delim2);
            diceRollStr1 = rollDice.substr(0, pos1);
            rollDice.erase(0, pos1 + delim2.length());
            diceRollStr2 = rollDice;
            //get integers
            constant = std::stoi(constantStr);
            numDice = std::stoi(diceRollStr1);
            numFaces = std::stoi(diceRollStr2); //max roll of the die
            int min = 1; //minimum roll of a die
            
            //generate bounded random
            std::random_device roll;
            std::mt19937 gen(roll());
            std::uniform_int_distribution<> distrib(min, numFaces); //creates norm_distr generator object
            
            //roll the dice
            for(int i = 0; i != numDice; i++) {
                diceRoll += distrib(gen);
            }

            //add constant
            diceRoll += constant;
            return diceRoll;
        }
        void display() {
            std::cout << NAME << std::endl;
            std::cout << DESC << std::endl;
            std::cout << SYMB << std::endl;
            for(auto c : COLOR) {
                std::cout << c + " " << std::ends;
            } std::cout << std::endl;
            std::cout << speedDice + " (rolled: " << SPEED << ")" << std::endl;
            std::cout << ABIL << std::endl;
            std::cout << hpDice << " (rolled: " << HP << ")" << std::endl;
            std::cout << damDice << " (rolled: " << DAM << ")" << std::endl;
            std::cout << RRTY << std::endl;
        }

        Monster(std::string name, std::string desc, std::string color, std::string speed, std::string abil, std::string hp, std::string dam, std::string symb, std::string rrty) {
            this->NAME = name;
            this->DESC = desc;
            this->COLOR.push_back(color);
            this->SPEED = rollDice(speed);
            this->speedDice = speed;
            this->ABIL = abil;
            this->HP = rollDice(hp);
            this->hpDice = hp;
            this->DAM = rollDice(dam);
            this->damDice = dam;
            this->SYMB = symb[0];//relying on the symbol being 1 byte long
            this->RRTY = std::stoi(rrty);
        };
        Monster() {
            //default constructor
        }
        ~Monster(void) {
            //std::cout << "Monster deconstructed" << std::endl;
        }
};

class Parser {
    public:
        int checkAmountOfMonstersFromFile(std::string fileName) {
            std::ifstream file(fileName);
            std::string data = "";
            int count = 0;
            if(!file.is_open()) {
                std::cerr << "Error in opening " << fileName << std::endl;
            }
            while(std::getline(file, data)) {
                if(data == "BEGIN MONSTER") {
                    count++;
                }
            }
            file.close();
            return count;
        }

        void extractor(std::string &line, std::string &attr, std::string marker) {
            //fields
            std::string currentLine = line;
            std::string delimiter = " ";
            std::string trash;

            //extracting logic
            auto pos = currentLine.find(delimiter);
            while(pos != std::string::npos) {
                if(currentLine.substr(0, pos) != marker) {
                    attr.append(currentLine.substr(0, pos) + " ");
                    currentLine.erase(0, pos + delimiter.length());
                    pos = currentLine.find(delimiter);
                }
                else {
                    trash.append(currentLine.substr(0, pos) + " ");
                    currentLine.erase(0, pos + delimiter.length());
                    pos = currentLine.find(delimiter);
                }
            }
            attr.append(currentLine);
        }

        Monster * fieldExtractor(std::string &startingLinePos, std::ifstream &file) {
            std::string delimiter = " ";
            std::string attributeCheck;
            std::string currentLine = startingLinePos;
            std::vector<std::string> brokenLine;
            std::string NAME, SYMB, COLOR, DESC, SPEED, DAM, HP, RRTY, ABIL;
            bool NAME_FLAG, SYMB_FLAG, COLOR_FLAG, DESC_FLAG, SPEED_FLAG, DAM_FLAG, HP_FLAG, RRTY_FLAG, ABIL_FLAG = false;

            //read through given section
            while(currentLine != "END") {
               auto pos = currentLine.find(delimiter);
                if(currentLine.substr(0,pos) == "NAME") {
                    extractor(currentLine, NAME, "NAME");
                    if(NAME_FLAG == true) NAME_FLAG = false;
                    NAME_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "SYMB") {
                    extractor(currentLine, SYMB, "SYMB");
                    if(SYMB_FLAG == true) SYMB_FLAG = false;
                    SYMB_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "COLOR") {
                    extractor(currentLine, COLOR, "COLOR");
                    if(COLOR_FLAG == true) COLOR_FLAG = false;
                    COLOR_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "DESC") {
                    std::getline(file, currentLine);
                    while(currentLine != ".") {
                        DESC.append(currentLine + " ");
                        std::getline(file, currentLine);
                    }
                    if(DESC.size() > 79) (DESC_FLAG = false); //cannot be larger than 78
                    if(DESC_FLAG == true) DESC_FLAG = false;
                    DESC_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "SPEED") {
                    extractor(currentLine, SPEED, "SPEED");
                    if(SPEED_FLAG == true) SPEED_FLAG = false;
                    SPEED_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "DAM") {
                    extractor(currentLine, DAM, "DAM");
                    if(DAM_FLAG == true) DAM_FLAG = false;
                    DAM_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "HP") {
                    extractor(currentLine, HP, "HP");
                    if(DAM_FLAG == true) DAM_FLAG = false;
                    HP_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "RRTY") {
                    extractor(currentLine, RRTY, "RRTY");
                    if(RRTY_FLAG == true) RRTY_FLAG = false;
                    RRTY_FLAG = true;
                }
                else if(currentLine.substr(0,pos) == "ABIL") {
                    extractor(currentLine, ABIL, "ABIL");
                    if(ABIL_FLAG == true) ABIL_FLAG = false;
                    ABIL_FLAG = true;
                }
                std::getline(file, currentLine);
            }
            if(!NAME_FLAG && !SYMB_FLAG && !COLOR_FLAG && !DESC_FLAG && !SPEED_FLAG && !DAM_FLAG && !HP_FLAG && !RRTY_FLAG && !ABIL_FLAG) {
                return NULL;
            }
            Monster* newMonster = new Monster(NAME, DESC, COLOR, SPEED, ABIL, HP, DAM, SYMB, RRTY);
            return newMonster;
        }

        std::vector<Monster> * readMonstersFromFile(std::string fileName) {
            std::vector<Monster>* mList = new std::vector<Monster>[checkAmountOfMonstersFromFile(fileName)];
            std::string del = " ";
            std::string extract;
            std::ifstream file(fileName);
            if(!file.is_open()) {
                std::cerr << "Error in opening " << fileName << std::endl;
            }
            std::string data = "";
            bool correctFormat = true;
            while(std::getline(file, data)) {
                if(!correctFormat) {
                    if(data == "RLG327 MONSTER DESCRIPTION 1" || "rlg327 monster description 1") {
                        correctFormat = true;
                    }
                    else {
                        std::cout << "File was not to specifications" << std::endl;
                        break;
                    }
                }
                else if(data == "BEGIN MONSTER") {
                    mList->push_back(*fieldExtractor(data, file));
                }
            }
            file.close();
            return mList;
        }     
};


int main() {
    //declaring variable to store filename
    std::string givenFileName;
    //extracting user input
    std::cout << "Must be in same folder as parser.cpp" << std::endl;
    std::cout << "Please enter in <filename>.<file-extension> : " << std::ends;    
    std::cin >> givenFileName;
    Parser parser;
    std::vector<Monster>* monsterList = parser.readMonstersFromFile(givenFileName);
    for(auto &monster : *monsterList) {
        std::cout << std::endl;
        monster.display();
    }
    delete[] monsterList;
    return 0;
};