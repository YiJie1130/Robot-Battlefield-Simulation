#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
#include <vector>
#include <deque>
using namespace std;

class Battlefield;
template <class Q>
class Queue;
template <class Q>
class CircularLinklist;
class Robot;
class MovingRobot;
class ShootingRobot;
class SeeingRobot;
class SteppingRobot;
class RoboCop;
class Terminator;
class TerminatorRobocop;
class BlueThunder;
class Madbot;
class RoboTank;
class UltimateRobot;
Robot *getRobot(const string &type, int rows, int cols, const vector<Robot *> &robots);
Robot *upgradeRobot(int type, int rows, int cols, const vector<Robot *> &robots);

template <class Q>
class Queue
{
    template <class N>
    struct Node
    {
        Robot *robot;
        Node<N> *next;
        Node(N x)
        {
            robot = x;
            next = nullptr;
        }
    };

    Node<Q> *front;
    Node<Q> *rear;
    int sz;

public:
    Queue()
    {
        sz = 0;
        front = rear = nullptr;
    }

    Queue(const Queue<Q> &other)
    {
        sz = 0;
        front = rear = nullptr;
        Node<Q> *ptr = other.front;
        while (ptr != nullptr)
        {
            enqueue(ptr->robot);
            ptr = ptr->next;
        }
    }

    bool isEmpty()
    {
        if (front == nullptr)
            return true;
        else
            return false;
    }

    Q dequeue()
    {
        if (!isEmpty())
        {
            Node<Q> *p = front;
            Q x = front->robot;
            front = front->next;
            delete p;
            sz--;
            return x;
        }
        else
        {
            cout << "Queue is empty\n";
            return nullptr;
        }
    }

    void enqueue(Q value)
    {
        if (isEmpty())
        {
            front = new Node<Q>(value);
            rear = front;
        }
        else
        {
            rear->next = new Node<Q>(value);
            rear = rear->next;
        }
        sz++;
    }

    int size()
    {
        return sz;
    }

    friend ostream &operator<<(ostream &out, const Queue<Q> &q)
    {
        Node<Q> *current = q.front;
        out << "-->";
        while (current != nullptr)
        {
            out << current->robot << " ";
            current = current->next;
        }
        out << endl;
        return out;
    }
};

class Robot
{
private:
    static int counter;

protected:
    int numkills = 0;
    int numlives = 3;
    int PositionX;
    int PositionY;
    string robotName;
    string robotType;
    Queue<Robot *> killedRobot;

public:
    int totalKill;
    bool isAlive = true;
    virtual void action(Battlefield &field) = 0;

    Robot() { counter++; }
    static int getCounter() { return counter;}
    // Getter and Setter for PositionX and PositionY
    void setPosition(int x, int y)
    {
        PositionX = x;
        PositionY = y;
    }

    int getPositionX() const { return PositionX; }
    int getPositionY() const { return PositionY; }

    void setRobotType(const string &t) { robotType = t; }
    string getRobotType() const { return robotType; }

    void setRobotName(const string &n) { robotName = n; }
    string getRobotName() const { return robotName; }

    void setNumLive(const int &l) { numlives = l; }
    int getNumLive() const { return numlives; }

    void setNumKill(int k) { numkills = k; }
    int getNumKill() const { return numkills; }
    void setTotalNumKill(int k) { totalKill = k; }
    int getTotalNumKill() const { return totalKill; }
    void resetKill() { numkills = 0; }

    void addtoQueue(Robot *robot)
    {
        killedRobot.enqueue(robot);
    }

    Queue<Robot *> *getRobotQueue()
    {
        return &killedRobot;
    }

    virtual void kill()
    {
        numkills++;
        totalKill++;
    }
    virtual void killed() { numlives--; }

    // virtual destructor for superclass
    virtual ~Robot() { counter--; }

};

int Robot::counter = 0;

template <class Q>
class CircularLinklist
{
    template <class N>
    struct Node
    {
        Robot *robot;
        Node *next;
    };

    Node<Q> *head; // head of the list
    int size;      // size of the list

    Node<Q> *getNode(Q value)
    {
        Node<Q> *newnode;
        newnode = new Node<Q>();
        newnode->robot = value;
        newnode->next = nullptr;
        return newnode;
    }

    Node<Q> *moveLast()
    {
        Node<Q> *last = head;
        while (last->next != head)
            last = last->next;
        return last;
    }

public:
    int getSize() { return size; }

    CircularLinklist()
    {
        head = nullptr;
        size = 0;
    }

    ~CircularLinklist()
    {
        size = 0;
        if (head != nullptr)
        {
            Node<Q> *current = head;
            Node<Q> *nextNode = nullptr;
            do
            {
                nextNode = current->next;
                delete current->robot;
                delete current;
                current = nextNode;
            } while (current != head);
        }
    }

    bool isEmpty()
    {
        if (head == nullptr)
            return true;
        else
            return false;
    }

    void push_back(Q value)
    {
        Node<Q> *newnode = getNode(value);
        if (head == nullptr)
        {
            head = newnode;
            newnode->next = head;
        }
        else
        {
            Node<Q> *last = moveLast();
            last->next = newnode;
            newnode->next = head;
        }
        size++;
    }

    void print(Battlefield &field)
    {
        if (head == nullptr)
        {
            cout << "List is empty" << endl;
            return;
        }

        Node<Q> *ptr = head;
        do
        {
            ptr->robot->action(field); // Call the action method of the Robot
            ptr = ptr->next;
        } while (ptr != head);
    }

    void erase(Q value)
    {
        Node<Q> *tobedeleted, *beforedeleted;

        if (!isEmpty())
        {
            if (head->next == head && value == head->robot)
            {
                Node<Q> *ptr = head;
                head = nullptr;
                delete ptr->robot;
                delete ptr;
            }
            else
            {
                tobedeleted = head->next;
                beforedeleted = head;
                while (tobedeleted->robot != value && tobedeleted != head)
                {
                    beforedeleted = tobedeleted;
                    tobedeleted = tobedeleted->next;
                }
                if (tobedeleted == head)
                    cout << "Element not found" << endl;
                else
                {
                    beforedeleted->next = tobedeleted->next;
                    delete tobedeleted->robot;
                    delete tobedeleted;
                }
            }
            size--;
        }
        else
        {
            cout << "List is empty" << endl;
        }
    }

    Node<Q> *getHead() const { return head; }

    void checkDead()
    {
        Node<Q> *ptr = head;
        do
        {
            if (ptr->robot->getNumLive() == 0)
            {
                cout << ptr->robot->getRobotName() << "(" << ptr->robot->getRobotType()
                     << ") lives = 0, it has been destroyed" << endl;
                erase(ptr->robot);
                ptr = head;
            }
            ptr = ptr->next;
        } while (ptr != head);
    }

    void queueRevive(Queue<Q> &q)
    {
        Node<Q> *ptr = head;
        do
        {
            if (!ptr->robot->isAlive)
            {
                cout << ptr->robot->getRobotName() << "(" << ptr->robot->getRobotType() << ") is in revive queue" << endl;
                q.enqueue(ptr->robot);
                erase(ptr->robot);
                ptr = head;
            }
            ptr = ptr->next;
        } while (ptr != head);
    }
};

const int MAX_BATTLEFIELD_SIZE = 100;

class Battlefield
{
private:
    int rows, cols;
    string field[MAX_BATTLEFIELD_SIZE][MAX_BATTLEFIELD_SIZE];
    int step;
    vector<Robot *> robots;
    CircularLinklist<Robot *> lst;

public:
    Battlefield(const string &filename, CircularLinklist<Robot *> &lst)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Error opening file: " << filename << endl;
            exit(1);
        }

        string lines;
        string line;

        // Read the M by N : rows cols line
        getline(file, lines);
        istringstream iss1(lines);
        iss1 >> line >> line >> line >> rows >> cols;
        cout << "M by N: " << rows << "  " << cols << endl;

        // Read the steps: steps line
        getline(file, lines);
        istringstream iss2(lines);
        iss2 >> line >> step;
        cout << "steps: " << step << endl;

        // Read the robots: count line
        getline(file, lines);
        istringstream iss3(lines);
        int robotCount;
        iss3 >> line >> robotCount;
        cout << "robots: " << robotCount << endl;

        for (int i = 0; i < robotCount; ++i)
        {
            string type, name, x_str, y_str;
            int x, y;
            file >> type >> name >> x_str >> y_str;
            cout << type << " " << name << " " << x_str << " " << y_str << "\n";

            // Convert x and y to integers or handle 'random'
            if (x_str == "random")
            {
                do
                {
                    x = rand() % rows;
                    y = rand() % cols; // Ensure y is initialized
                } while (isPositionOccupied(x, y));
            }
            else
            {
                x = stoi(x_str);
            }

            if (y_str == "random")
            {
                do
                {
                    y = rand() % cols;
                } while (isPositionOccupied(x, y));
            }
            else
            {
                y = stoi(y_str);
            }

            Robot *robot = getRobot(type, rows, cols, robots);
            robot->setPosition(x, y);
            robot->setRobotType(type);
            robot->setRobotName(name);
            robots.push_back(robot);
            lst.push_back(robot);
        }

        file.close();
        initializeField();
        placeRobots();
    }

    int getRows() { return rows; }
    int getCols() { return cols; }
    int getStep() const { return step; }

    bool isPositionOccupied(int x, int y)
    {
        if (!boundCheck(x, y))
            return false;

        for (const auto &robot : robots)
        {
            if (robot->getPositionX() == x && robot->getPositionY() == y)
                return true;
        }
        return false;
    }

    Robot *robotGetKilled(int x, int y)
    {
        for (const auto &robot : robots)
        {
            if (robot->getPositionX() == x && robot->getPositionY() == y)
            {
                return robot;
            }
        }
        return nullptr;
    }

    void upgrade(Robot *&robot, CircularLinklist<Robot *> lst)
    {
        if (robot->getNumKill() == 3)
        {
            robot->resetKill();

            // transfer all value to upgraded robot
            int x = robot->getPositionX();
            int y = robot->getPositionY();
            int live = robot->getNumLive();
            string name = robot->getRobotName();
            int totalKill = robot->getTotalNumKill();

            if (robot->getRobotType() == "RoboCop" || robot->getRobotType() == "Terminator")
            {
                delete robot;
                robot = upgradeRobot(1, rows, cols, robots);
                robot->setRobotType("TerminatorRoboCop");
            }
            else if (robot->getRobotType() == "BlueThunder")
            {
                delete robot;
                robot = upgradeRobot(2, rows, cols, robots);
                robot->setRobotType("Madbot");
            }
            else if (robot->getRobotType() == "Madbot")
            {
                delete robot;
                robot = upgradeRobot(3, rows, cols, robots);
                robot->setRobotType("RoboTank");
            }
            else if (robot->getRobotType() == "TerminatorRobocop" || robot->getRobotType() == "RoboTank")
            {
                delete robot;
                robot = upgradeRobot(4, rows, cols, robots);
                robot->setRobotType("UltimateRobot");
            }
            robot->setPosition(x, y);
            robot->setNumLive(live);
            robot->setRobotName(name);
            robot->setTotalNumKill(totalKill);

            cout << name << " is upgraded to " << robot->getRobotType() << endl
                 << endl;
        }
    }

    bool boundCheck(int x, int y)
    {
        if ((x < 0) || (x > rows) || (y < 0) || (y > cols))
            return false;
        else
            return true;
    }

    void initializeField()
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                field[i][j] = "";
            }
        }
    }

    void placeRobots()
    {
        for (const auto &robot : robots)
        {
            int x = robot->getPositionX();
            int y = robot->getPositionY();
            if (x >= 0 && x < rows && y >= 0 && y < cols)
            {
                field[x][y] = robot->getRobotName();
            }
        }
    }

    void updateField()
    {
        initializeField();
        placeRobots();
    }

    void displayField() const
    {
        cout << "   ";
        for (int j = 0; j < cols; ++j)
        {
            cout << " " << right << setfill('0') << setw(2) << j << "  ";
        }
        cout << endl;

        for (int i = 0; i < rows; ++i)
        {
            cout << "  ";
            for (int j = 0; j < cols; ++j)
            {
                cout << "+----";
            }
            cout << "+" << endl;

            cout << right << setfill('0') << setw(2) << i;
            for (int j = 0; j < cols; j++)
            {
                cout << "|";
                if (field[i][j] == "")
                {
                    cout << "    ";
                }
                else
                {
                    cout << " " << left << setw(2) << field[i][j].substr(0, 2) << " ";
                }
            }
            cout << "|" << endl;
        }

        cout << "  ";
        for (int j = 0; j < cols; ++j)
        {
            cout << "+----";
        }
        cout << "+" << endl;
    }

    void updateAndDisplay()
    {
        printRobotQueues();
        updateField();
        displayField();
    }

    void printRobotQueues()
    {
        for (auto &robot : robots)
        {
            Queue<Robot *> *queue = robot->getRobotQueue();
            while (!queue->isEmpty())
            {
                Robot *enqueuedRobot = queue->dequeue();
                eraseRobot(enqueuedRobot->getRobotName(), enqueuedRobot->getRobotType());
            }
            cout << endl;
        }
    }

    void eraseRobot(const string &name, const string &type)
    {
        for (auto it = robots.begin(); it != robots.end(); it++)
        {
            if ((*it)->getRobotName() == name)
            {
                delete *it;
                it = robots.erase(it);
                break;
            }
            if ((*it)->getRobotType() == type)
            {
                delete *it;
                lst.erase(*it);
                cout << *it << " deleted in list" << endl;
                break;
            }
        }
        placeRobots();
    }

    void UpdatedBattlefield(CircularLinklist<Robot *> lst, Battlefield &battlefield, Queue<Robot *> &q, Robot *&robot)
    {
        auto ptr = lst.getHead(); // Get the head of the list

        do
        {
            ptr->robot->action(battlefield); // Perform the robot's action
            battlefield.updateAndDisplay();  // Update and display the battlefield

            ptr = ptr->next; // Move to the next node
        } while (ptr != lst.getHead()); // Loop until back at the head of the list

    }

    void updatedbattlefield(CircularLinklist<Robot *> lst, Battlefield &battlefield, Queue<Robot *> &q, Robot *&robot)
    {
        for (int i = 0; i < battlefield.getStep(); i++)
        {
            cout << endl;
            cout << "_____________________Turn " << i + 1 << "_____________________" << endl;
            cout << endl;

            battlefield.UpdatedBattlefield(lst, battlefield, q, robot);

            // check dead and queue revive
            lst.checkDead();
            lst.queueRevive(q);
            // check upgrade
            battlefield.upgrade(robot, lst);
        }
    }
};

class MovingRobot : virtual public Robot
{
protected:
    int moveX;
    int moveY;
    int rows; // Battlefield rows
    int cols; // Battlefield cols
    const vector<Robot *> &robots;

public:
    MovingRobot(int r, int c, const vector<Robot *> &robots) : rows(r), cols(c), robots(robots) {}
    virtual void move(Battlefield &field) = 0;
    virtual ~MovingRobot() {}

    string moving(Battlefield &field)
    {
        bool moved = false;

        while (!moved)
        {
            int direction = rand() % 8;
            switch (direction)
            {
            case 0: // up left
                moveX = -1;
                moveY = -1;
                break;
            case 1: // up
                moveX = -1;
                moveY = 0;
                break;
            case 2: // up-right
                moveX = -1;
                moveY = 1;
                break;
            case 3: // left
                moveX = 0;
                moveY = -1;
                break;
            case 4: // right
                moveX = 0;
                moveY = 1;
                break;
            case 5: // down left
                moveX = 1;
                moveY = -1;
                break;
            case 6: // down
                moveX = 1;
                moveY = 0;
                break;
            case 7: // down right
                moveX = 1;
                moveY = 1;
                break;
            }
            moveX = getPositionX() + moveX;
            moveY = getPositionY() + moveY;

            // Ensure new position is within bounds and not occupied
            if (field.boundCheck(moveX, moveY) && !field.isPositionOccupied(moveX, moveY))
            {
                setPosition(moveX, moveY);
                moved = true;
            }
        }
        return getRobotName() + " (" + getRobotType() + ") moved to (" +
               to_string(getPositionX()) + ", " + to_string(getPositionY()) + ")";
    }
};

// me
class ShootingRobot : virtual public Robot
{
protected:
    int rows; // Battlefield rows
    int cols; // Battlefield cols
    int shootX;
    int shootY;
    const vector<Robot *> &robots;

public:
    ShootingRobot(int r, int c, const vector<Robot *> &robots) : rows(r), cols(c), robots(robots) {}

    virtual void fire(Battlefield &field) = 0;
    virtual ~ShootingRobot() {}
};

class SeeingRobot : virtual public Robot
{
protected:
    int seeX;
    int seeY;
    int rows; // Battlefield rows
    int cols; // Battlefield cols
    Robot *detectedRobot;
    vector<vector<int>> nearby;
    const vector<Robot *> &robots;

public:
    SeeingRobot(int r, int c, const vector<Robot *> &robots)
        : rows(r), cols(c), robots(robots), detectedRobot(nullptr) {}

    void setDetectedRobot(Robot *robot) { detectedRobot = robot; }
    Robot *getDetectedRobot() const { return detectedRobot; }

    virtual bool look() = 0;
    virtual ~SeeingRobot() {}

    bool looking()
    {
        seeX = getPositionX() - 1;
        seeY = getPositionY() - 1;

        nearby.clear(); // Clear the vector to store new nearby positions

        // Loop through the 3x3 grid centered on the robot's position to gather nearby positions
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                int checkX = seeX + i;
                int checkY = seeY + j;
                if (checkX == getPositionX() && checkY == getPositionY())
                {
                    continue; // Skip the robot's current position
                }
                // Check if the position is within battlefield bounds
                if (checkX >= 0 && checkX < rows && checkY >= 0 && checkY < cols)
                {
                    nearby.push_back({checkX, checkY}); // Add the position to nearby for display purposes
                }
            }
        }

        // Display the nearby positions
        cout << getRobotName() << " (" << getRobotType() << ") nearby positions: ";
        for (const auto &pos : nearby)
        {
            cout << "(" << pos[0] << ", " << pos[1] << ") ";
        }
        cout << endl;

        // Check for enemies and display message if an enemy is found
        for (const auto &pos : nearby)
        {
            int checkX = pos[0];
            int checkY = pos[1];

            // Check if there is another robot (enemy) at this position
            for (const auto &robot : robots)
            {
                if (robot->getPositionX() == checkX && robot->getPositionY() == checkY)
                {
                    setDetectedRobot(robot);
                    cout << getRobotName() << " (" << getRobotType() << ") detected enemy at position (" << checkX << ", " << checkY << ")!" << endl;
                    break; // Exit the loop if enemy is found
                }
            }
        }

        // check if have at least one robot nearby
        if (getDetectedRobot() != nullptr)
            return true;
        else
            return false;
    }
};

class SteppingRobot : virtual public Robot
{
protected:
    int stepX;
    int stepY;

public:
    virtual void step(Battlefield &field) = 0;
    virtual ~SteppingRobot() {}

    string stepping(int x, int y)
    {
        setPosition(x, y);
        return getRobotName() + " (" + getRobotType() + ") stepped to (" +
               to_string(getPositionX()) + ", " + to_string(getPositionY()) + ")";
    }
};

class RoboCop : virtual public MovingRobot, virtual public ShootingRobot, virtual public SeeingRobot
{
public:
    RoboCop(int r, int c, const vector<Robot *> &robots)
        : MovingRobot(r, c, robots), ShootingRobot(r, c, robots), SeeingRobot(r, c, robots) {}
    virtual bool look() override { return looking(); }
    virtual void move(Battlefield &field) override { cout << moving(field) << endl; }
    virtual void fire(Battlefield &field) override
    {
        bool inRange = false;
        while (!inRange)
        {
            int randomX = rand() % 10;
            int randomY = rand() % 10;

            int negativeNum = rand() % 4;
            switch (negativeNum)
            {
            case 0:
                randomX = (randomX * -1);
                break;
            case 1:
                randomY = (randomY * -1);
                break;
            case 2:
                randomX = (randomX * -1);
                randomY = (randomY * -1);
                break;
            }
            shootX = randomX + getPositionX();
            shootY = randomY + getPositionY();
            inRange = ((abs(randomX) + abs(randomY) <= 10) && (shootX != getPositionX() && (shootY != getPositionY())) && field.boundCheck(shootX, shootY));
        }

        cout << getRobotName() << " (" + getRobotType() + ") shot at (" << shootX << ", " << shootY << ")" << endl;
        if (field.robotGetKilled(shootX, shootY) != nullptr)
        {
            Robot *robotGetKilled = field.robotGetKilled(shootX, shootY);
            this->addtoQueue(robotGetKilled);
            robotGetKilled->isAlive = false;
            robotGetKilled->killed();
            cout << robotGetKilled->getRobotName() << " (" << robotGetKilled->getRobotType() << ") get killed" << endl;
            kill();
        }
    }

    virtual void action(Battlefield &field) override
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        look();
        move(field);
        fire(field);
        fire(field);
        fire(field);
        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

class Terminator : virtual public MovingRobot, virtual public SeeingRobot, virtual public SteppingRobot
{
protected:
    Robot *nearby;

public:
    Terminator(int r, int c, const vector<Robot *> &robots)
        : MovingRobot(r, c, robots), SeeingRobot(r, c, robots), nearby(nullptr) {}
    virtual bool look() override
    {
        bool detectedRobot = looking();
        nearby = getDetectedRobot();
        return detectedRobot;
    }
    virtual void move(Battlefield &field) override { cout << moving(field) << endl; }
    virtual void step(Battlefield &field) override
    {
        stepX = nearby->getPositionX();
        stepY = nearby->getPositionY();

        cout << getRobotName() << " (" + getRobotType() + ") stepped at (" << stepX << ", " << stepY << ")" << endl;
        if (field.robotGetKilled(stepX, stepY) != nullptr)
        {
            Robot *robotGetKilled = field.robotGetKilled(stepX, stepY);
            this->addtoQueue(robotGetKilled);
            robotGetKilled->isAlive = false;
            robotGetKilled->killed();
            cout << robotGetKilled->getRobotName() << " (" << robotGetKilled->getRobotType() << ") get killed" << endl;
            kill();
        }
    }
    virtual void action(Battlefield &field) override
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        // perform step when detect enemy, otherwise just move once
        if (look())
            step(field);
        else
            move(field);

        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

class TerminatorRobocop : virtual public RoboCop, virtual public Terminator
{
public:
    TerminatorRobocop(int r, int c, const vector<Robot *> &robots)
        : MovingRobot(r, c, robots), ShootingRobot(r, c, robots), SeeingRobot(r, c, robots),
          RoboCop(r, c, robots), Terminator(r, c, robots) {}
    virtual void move(Battlefield &field) override
    {
        Terminator::move(field);
    }
    virtual bool look() override
    {
        return Terminator::look();
    }
    void action(Battlefield &field)
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        // Terminator will check
        Terminator::action(field);
        RoboCop::fire(field);
        RoboCop::fire(field);
        RoboCop::fire(field);
        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

class BlueThunder : virtual public ShootingRobot
{
protected:
    int turn = 0;

public:
    BlueThunder(int r, int c, const vector<Robot *> &robots) : ShootingRobot(r, c, robots) {}
    virtual void fire(Battlefield &field) override
    {
        bool inRange = false;
        while (!inRange)
        {
            int clockwise = turn % 8;
            switch (clockwise)
            {
            case 0: // up
                shootX = -1;
                shootY = 0;
                break;
            case 1: // up-right
                shootX = -1;
                shootY = 1;
                break;
            case 2: // right
                shootX = 0;
                shootY = 1;
                break;
            case 3: // down right
                shootX = 1;
                shootY = 1;
                break;
            case 4: // down
                shootX = 1;
                shootY = 0;
                break;
            case 5: // down left
                shootX = 1;
                shootY = -1;
                break;
            case 6: // left
                shootX = 0;
                shootY = -1;
                break;
            case 7: // up left
                shootX = -1;
                shootY = -1;
                break;
            }
            shootX = shootX + getPositionX();
            shootY = shootY + getPositionY();
            inRange = (field.boundCheck(shootX, shootY));
            turn++;
        }

        cout << getRobotName() << " (" + getRobotType() + ") shot at (" << shootX << ", " << shootY << ")" << endl;
        if (field.robotGetKilled(shootX, shootY) != nullptr)
        {
            Robot *robotGetKilled = field.robotGetKilled(shootX, shootY);
            this->addtoQueue(robotGetKilled);
            robotGetKilled->isAlive = false;
            robotGetKilled->killed();
            cout << robotGetKilled->getRobotName() << " (" << robotGetKilled->getRobotType() << ") get killed" << endl;
            kill();
        }
    }
    virtual void action(Battlefield &field) override
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        fire(field);
        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

class Madbot : virtual public BlueThunder
{
public:
    Madbot(int r, int c, const vector<Robot *> &robots)
        : BlueThunder(r, c, robots), ShootingRobot(r, c, robots) {}
    virtual void fire(Battlefield &field) override
    {
        bool inRange = false;
        while (!inRange)
        {
            int direction = rand() % 8;
            switch (direction)
            {
            case 0: // up
                shootX = -1;
                shootY = 0;
                break;
            case 1: // up-right
                shootX = -1;
                shootY = 1;
                break;
            case 2: // right
                shootX = 0;
                shootY = 1;
                break;
            case 3: // down right
                shootX = 1;
                shootY = 1;
                break;
            case 4: // down
                shootX = 1;
                shootY = 0;
                break;
            case 5: // down left
                shootX = 1;
                shootY = -1;
                break;
            case 6: // left
                shootX = 0;
                shootY = -1;
                break;
            case 7: // up left
                shootX = -1;
                shootY = -1;
                break;
            }
            shootX = shootX + getPositionX();
            shootY = shootY + getPositionY();
            inRange = (field.boundCheck(shootX, shootY));
        }

        cout << getRobotName() << " (" + getRobotType() + ") shot at (" << shootX << ", " << shootY << ")" << endl;
        if (field.robotGetKilled(shootX, shootY) != nullptr)
        {
            Robot *robotGetKilled = field.robotGetKilled(shootX, shootY);
            this->addtoQueue(robotGetKilled);
            robotGetKilled->isAlive = false;
            robotGetKilled->killed();
            cout << robotGetKilled->getRobotName() << " (" << robotGetKilled->getRobotType() << ") get killed" << endl;
            kill();
        }
    }

    virtual void action(Battlefield &field) override
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        fire(field);
        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

class RoboTank : virtual public Madbot
{
public:
    RoboTank(int r, int c, const vector<Robot *> &robots)
        : Madbot(r, c, robots), BlueThunder(r, c, robots), ShootingRobot(r, c, robots) {}
    virtual void fire(Battlefield &field) override
    {
        bool inRange = false;
        while (!inRange)
        {
            int randomX = rand() % (field.getRows());
            int randomY = rand() % (field.getCols());

            int negativeNum = rand() % 4;
            switch (negativeNum)
            {
            case 0:
                randomX = (randomX * -1);
                break;
            case 1:
                randomY = (randomY * -1);
                break;
            case 2:
                randomX = (randomX * -1);
                randomY = (randomY * -1);
                break;
            }
            shootX = randomX + getPositionX();
            shootY = randomY + getPositionY();
            inRange = ((abs(randomX) + abs(randomY) != 0) && field.boundCheck(shootX, shootY));
        }

        cout << getRobotName() << " (" + getRobotType() + ") shot at (" << shootX << ", " << shootY << ")" << endl;
        if (field.robotGetKilled(shootX, shootY) != nullptr)
        {
            Robot *robotGetKilled = field.robotGetKilled(shootX, shootY);
            this->addtoQueue(robotGetKilled);
            robotGetKilled->isAlive = false;
            robotGetKilled->killed();
            cout << robotGetKilled->getRobotName() << " (" << robotGetKilled->getRobotType() << ") get killed" << endl;
            kill();
        }
    }
    virtual void action(Battlefield &field) override
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        fire(field);
        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

class UltimateRobot : public TerminatorRobocop, public RoboTank
{
public:
    UltimateRobot(int r, int c, const vector<Robot *> &robots)
        : MovingRobot(r, c, robots), ShootingRobot(r, c, robots), SeeingRobot(r, c, robots),
          RoboCop(r, c, robots), Terminator(r, c, robots), TerminatorRobocop(r, c, robots),
          Madbot(r, c, robots), BlueThunder(r, c, robots), RoboTank(r, c, robots) {}

    virtual void fire(Battlefield &field) override
    {
        RoboTank::fire(field);
    }
    void action(Battlefield &field)
    {
        if (!isAlive)
        {
            cout << endl;
            cout << getRobotName() + " (" + getRobotType() + ") is get killed, wait for revive" << endl
                 << endl;
            cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
                 << endl;
            return;
        }
        cout << endl;
        cout << getRobotName() + " (" + getRobotType() + ") at position (" << getPositionX() << "," << getPositionY() << ")" << endl;
        TerminatorRobocop::move(field);
        RoboTank::action(field);
        RoboTank::action(field);
        RoboTank::action(field);
        cout << endl;
        cout << getRobotName() << " kills= " << getNumKill() << ", lives= " << getNumLive() << endl
             << endl;
    }
};

// Robot Factory
Robot *getRobot(const string &type, int rows, int cols, const vector<Robot *> &robots)
{
    if (type == "RoboCop")
        return new RoboCop(rows, cols, robots);
    else if (type == "Terminator")
        return new Terminator(rows, cols, robots);
    else if (type == "TerminatorRoboCop")
        return new TerminatorRobocop(rows, cols, robots);
    else if (type == "BlueThunder")
        return new BlueThunder(rows, cols, robots);
    else if (type == "Madbot")
        return new Madbot(rows, cols, robots);
    else if (type == "RoboTank")
        return new RoboTank(rows, cols, robots);
    else if (type == "UltimateRobot")
        return new UltimateRobot(rows, cols, robots);
    else
    {
        cerr << "Unknown robot type: " << type << endl;
        exit(1);
    }
    return nullptr; // To avoid compiler warning
}

Robot *upgradeRobot(int type, int rows, int cols, const vector<Robot *> &robots)
{
    if (type == 1)
        return new TerminatorRobocop(rows, cols, robots);
    else if (type == 2)
        return new Madbot(rows, cols, robots);
    else if (type == 3)
        return new RoboTank(rows, cols, robots);
    else if (type == 4)
        return new UltimateRobot(rows, cols, robots);
    else
    {
        cerr << "Unknown robot type: " << type << endl;
        exit(1);
    }
    return nullptr; // To avoid compiler warning
}

int main()
{
    srand(time(0)); // Seed for random position generation

    Robot *robot;
    Queue<Robot *> q;
    CircularLinklist<Robot *> lst;
    Battlefield battlefield("Robot.txt", lst);
    cout << endl;
    battlefield.displayField();

    battlefield.updatedbattlefield(lst, battlefield, q, robot);

    return 0;
}