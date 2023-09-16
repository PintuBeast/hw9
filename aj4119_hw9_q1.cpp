#include<iostream>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<string>
using namespace std;

int antSize = 0, bugSize = 0;
const int INVALID_MOVE=99;
const int GRIDSIZE=20;
const int MAX_ANTS=100;
const int MAX_BUGS=5;
const int STEPS_TO_BREED_ANTS=3;
const int STEPS_TO_BREED_BUGS=8;
const int STEPS_TO_DIE_BUGS=3;
const int KICKED_OFF_WORLD=999;

class Organism
{
    protected:
        int X;
        int Y;
        int ID;
        bool killFlag;
    public:
        virtual void move(int& targetX, int& targetY);
        int getX();
        int getY();
        int getID();
        bool getkillFlag();        
        void setX(int X);
        void setY(int Y);
        void setID(int ID);
        void setkillFlag(bool killFlag);        


};

class Ant: public Organism
{
    private:
        int stepsSinceBreeding;
    public:
        Ant(int X, int Y, int ID);
        Ant();
        virtual void move(int& targetX, int& targetY);
        int getStepsSinceBreeding();
        void setStepsSinceBreeding(int stepsSinceBreeding);
        void checkBreed();
};



class Bug: public Organism
{
    private:
        int stepsSinceBreeding;
        int stepsSinceEating;
    public:
        Bug(int X, int Y, int ID);  
        Bug();  
        virtual void move(int& targetX, int& targetY);
        int getStepsSinceBreeding();
        void setStepsSinceBreeding(int stepsSinceBreeding);
        int getStepsSinceEating();
        void setStepsSinceEating(int stepsSinceEating);        
        // void checkEat();
        void checkStarve();
        void checkBreed();   
        
};

vector<Ant> antVector;
vector<Bug> bugVector;

void spawnAnt(int X, int Y);
void killAnts();
void spawnBug(int X, int Y);
void killBugs();
void findNextAntMove(const int &X, const int &Y, int &targetX, int &targetY);
void findNextBugMove(Bug &b, const int &X, const int &Y, int &targetX, int &targetY); 
bool checkEmpty(int X, int Y);
void init();

void displayWorld();
bool checkForAnt(const int &X, const int &Y);
bool checkForBug(const int &X, const int &Y);
void moveCritters();

//  function definitions for Class Organism -starts here
int Organism :: getX()
{
    
    return this->X;
}

int Organism :: getY()
{
    return this->Y;
}

int Organism :: getID()
{
    return this->ID;
}

bool Organism :: getkillFlag()
{
    return this->killFlag;
}

void Organism :: setX(int X)
{
    this->X = X;
}
void Organism :: setY(int Y)
{
    this->Y = Y;
}
void Organism :: setID(int ID)
{
    this->ID = ID;
}

void Organism :: setkillFlag(bool killFlag)
{
    this->killFlag = killFlag;
}

void Organism :: move(int& targetX, int& targetY)
{
    this->setX(targetX);
    this->setY(targetY);

}

//  function definitions for Class Organism -ends here

//  function definitions for Class Ant -starts here

Ant::Ant(int X, int Y, int ID)
{
    this->setID(ID);
    this->setX(X);
    this->setY(Y);
    this->setStepsSinceBreeding(0);
    this->setkillFlag(false);
}

Ant::Ant()
{
    this->setID(antSize);
    this->setX(KICKED_OFF_WORLD);
    this->setY(KICKED_OFF_WORLD);
    this->setStepsSinceBreeding(0);
    this->setkillFlag(false);
}

int Ant :: getStepsSinceBreeding()
{
    return this->stepsSinceBreeding;
}

void Ant :: setStepsSinceBreeding(int stepsSinceBreeding)
{
    this->stepsSinceBreeding = stepsSinceBreeding;
}

void Ant :: move(int& targetX, int& targetY)
{
    targetX = INVALID_MOVE;
    targetY = INVALID_MOVE;



    findNextAntMove(this->getX(), this->getY(), targetX, targetY);
    if(targetX != INVALID_MOVE && targetY != INVALID_MOVE)
    {

     
    
    this->setX(targetX);
    this->setY(targetY);
    }
 
    this->setStepsSinceBreeding(this->getStepsSinceBreeding() + 1);
    
    return;

}



void Ant :: checkBreed()
{
    
    int steps = this->getStepsSinceBreeding();
    if(steps>=STEPS_TO_BREED_ANTS)
    {

        int X = this->getX(), Y= this->getY();
        
        if(checkEmpty(X, Y-1))      // check if cell above is empty
        {
           spawnAnt(X,Y-1);
           
            this->setStepsSinceBreeding(0);
       
      
            return;
        }    
        else
        if(checkEmpty(X, Y+1))      // check if cell below is empty
        {
            spawnAnt(X,Y+1);
           
            this->setStepsSinceBreeding(0);

        
            return;
        } 
        else
        if(checkEmpty(X-1, Y))      // check if cell to the left is empty
        {
            spawnAnt(X-1,Y);
            this->setStepsSinceBreeding(0);
        
            return;
        }
        else
        if(checkEmpty(X+1, Y))      // check if cell to the right is empty
        {
            spawnAnt(X+1,Y);
            this->setStepsSinceBreeding(0);
      
            return;
        } 
       

        
    }


}

//  function definitions for Class Ant -ends here


//  function definitions for Class Bug -starts here
Bug::Bug(int X, int Y, int ID)
{
    this->setID(ID);
    this->setX(X);
    this->setY(Y);
    this->setStepsSinceBreeding(0);
    this->setStepsSinceEating(0);
    this->setkillFlag(false);    
}

Bug::Bug()
{
    this->setID(bugSize);
    this->setX(KICKED_OFF_WORLD);
    this->setStepsSinceEating(KICKED_OFF_WORLD);
    this->setkillFlag(false);    
}

int Bug :: getStepsSinceBreeding()
{
    return this->stepsSinceBreeding;
}

void Bug :: setStepsSinceBreeding(int stepsSinceBreeding)
{
    this->stepsSinceBreeding = stepsSinceBreeding;
}

int Bug :: getStepsSinceEating()
{
    return this->stepsSinceEating;
}

void Bug :: setStepsSinceEating(int stepsSinceEating)
{
    this->stepsSinceEating = stepsSinceEating;
}


void Bug :: move(int& targetX, int& targetY)
{
    targetX = INVALID_MOVE;
    targetY = INVALID_MOVE;

 
    findNextBugMove(*this, this->getX(), this->getY(), targetX, targetY);
    
    if(targetX != INVALID_MOVE && targetY != INVALID_MOVE)
    {

 
   
    this->setX(targetX);
    this->setY(targetY);
    }

    
    

}






void Bug :: checkStarve()
{
    int steps = this->getStepsSinceEating();
    if(steps>STEPS_TO_DIE_BUGS)
    {
 
    }
    else
    if(steps == STEPS_TO_DIE_BUGS)
    {
  
        this->setkillFlag(true);
        this->setX(KICKED_OFF_WORLD);
        this->setY(KICKED_OFF_WORLD);        
    }
}

void Bug :: checkBreed()
{

    int steps = this->getStepsSinceBreeding();
   

    if(steps>=STEPS_TO_BREED_BUGS)
    {
   
        int X = this->getX(), Y= this->getY();
        
        if(checkEmpty(X, Y-1))      // check if cell above is empty
        {
            spawnBug(X,Y-1);
            this->setStepsSinceBreeding(0);
   
            return;
        }    
        else
        if(checkEmpty(X, Y+1))      // check if cell below is empty
        {
            spawnBug(X,Y+1);
            this->setStepsSinceBreeding(0);
   
            return;
        } 
        else
        if(checkEmpty(X-1, Y))      // check if cell to the left is empty
        {
            spawnBug(X-1,Y);
            this->setStepsSinceBreeding(0);
    
            return;
        }
        else
        if(checkEmpty(X+1, Y))      // check if cell to the right is empty
        {
            spawnBug(X+1,Y);
            this->setStepsSinceBreeding(0);
    
            return;
        } 
        
    }
  

}


//  function definitions for Class Bug -ends here

//  Common function definitions starts here
void spawnAnt(int X, int Y)
{

  
    Ant ant (X, Y, antSize);
    antVector.push_back(ant);
    antSize++;

}
void killAnts()
{
    for(int i=0;i<antSize;i++)
    {
        if(antVector[i].getkillFlag() == true)
        {
            for(int j=i;j<antSize-1;j++)
            {
                antVector[j]=antVector[j+1];
            }
            antVector.erase(antVector.begin() + antSize-1);
        }
    }
    antSize=antVector.size();

    int antID=0;
    for(int i=0;i<antSize;i++)
    {
        antVector[i].setID(antID++);
    }


}


void spawnBug(int X, int Y)
{ 
   
  
    Bug bug (X, Y, bugSize);
    
    bugVector.push_back(bug);
    bugSize++;
    

}
void killBugs()
{  
    for(int i=0;i<bugSize;i++)
    {
        if(bugVector[i].getkillFlag() == true)
        {
            for(int j=i;j<bugSize-1;j++)
            {
                bugVector[j]=bugVector[j+1];
            }
            bugVector.erase(bugVector.begin() + bugSize-1);
        }
    }
    bugSize=bugVector.size();

    int bugID=0;
    for(int i=0;i<bugSize;i++)
    {
        bugVector[i].setID(bugID++);
    }

}


void findNextAntMove(const int &X, const int &Y, int &targetX, int &targetY)
{
    targetX=INVALID_MOVE; //initialising with invalid values
    targetY=INVALID_MOVE;

    if(checkEmpty(X, Y-1))      // check if cell above is empty
        {
            targetX = X; 
            targetY= Y-1;
            
            return;
        }    
        else
        if(checkEmpty(X, Y+1))      // check if cell below is empty
        {
            targetX = X; 
            targetY= Y+1;
            
            return;
        } 
        else
        if(checkEmpty(X-1, Y))      // check if cell to the left is empty
        {
            targetX = X-1; 
            targetY= Y;
           
            return;
        }
        else
        if(checkEmpty(X+1, Y))      // check if cell to the right is empty
        {
            targetX = X+1; 
            targetY= Y;
            
            return;
        }  

}



void findNextBugMove(Bug &b, const int &X, const int &Y, int &targetX, int &targetY)
{
    
    b.setStepsSinceBreeding(b.getStepsSinceBreeding() + 1);
   
    for(int i=0;i<antSize;i++)
    {
        if( X == antVector[i].getX() && Y-1 == antVector[i].getY() )      // check ant is in cell above
        {
            targetX = X; 
            targetY= Y-1;
      
            antVector[i].setkillFlag(true);
            antVector[i].setX(KICKED_OFF_WORLD);
            antVector[i].setY(KICKED_OFF_WORLD);
            b.setStepsSinceEating(0);
            return;
        }
        else
        if(X == antVector[i].getX() && Y+1 == antVector[i].getY())      // check ant is in cell below 
        {
            targetX = X; 
            targetY= Y+1;
      
            antVector[i].setkillFlag(true);
            antVector[i].setX(KICKED_OFF_WORLD);
            antVector[i].setY(KICKED_OFF_WORLD); 
            b.setStepsSinceEating(0);           
            return;
        } 
        else
        if(X-1 == antVector[i].getX() && Y == antVector[i].getY())      // check ant is in cell to the left
        {
            targetX = X-1; 
            targetY= Y;
      
            antVector[i].setkillFlag(true);
            antVector[i].setX(KICKED_OFF_WORLD);
            antVector[i].setY(KICKED_OFF_WORLD);  
            b.setStepsSinceEating(0);          
            return;
        }    
        if(X+1 == antVector[i].getX() && Y == antVector[i].getY())      // check ant is in cell to the right
        {
            targetX = X+1; 
            targetY= Y;
     
            antVector[i].setkillFlag(true);
            antVector[i].setX(KICKED_OFF_WORLD);
            antVector[i].setY(KICKED_OFF_WORLD);   
            b.setStepsSinceEating(0);         
            return;
        }
    }

        b.setStepsSinceEating(b.getStepsSinceEating() + 1);      // if bug is not able to find any ant to eat
        
        if(checkEmpty(X, Y-1))      // check if cell above is empty
        {
            targetX = X; 
            targetY= Y-1;
            return;
        }    
        else
        if(checkEmpty(X, Y+1))      // check if cell below is empty
        {
            targetX = X; 
            targetY= Y+1;
            return;
        } 
        else
        if(checkEmpty(X-1, Y))      // check if cell to the left is empty
        {
            targetX = X-1; 
            targetY= Y;
            return;
        }
        else
        if(checkEmpty(X+1, Y))      // check if cell to the right is empty
        {
            targetX = X+1; 
            targetY= Y;
            return;
        }            

}




bool checkEmpty(int X, int Y)
{
    bool isEmpty=true;

    if(X >= GRIDSIZE || Y >= GRIDSIZE || X < 0 || Y <0)
    {
        isEmpty=false;
        return isEmpty;
    }

    for(int i=0;i<bugSize;i++)
    {
        if(X == bugVector[i].getX() && Y == bugVector[i].getY()) 
        {
            isEmpty=false;
            return isEmpty;
        }

    }

    for(int i=0;i<antSize;i++)
    {
        if(X == antVector[i].getX() && Y == antVector[i].getY()) 
        {
            isEmpty=false;
            return isEmpty;
        }

    } 

return isEmpty;
    
}


bool checkForAnt(const int &X, const int &Y)
{
    bool antPresent=false;
    for(int i=0;i<antSize;i++)
    {
        if(X == antVector[i].getX() && Y == antVector[i].getY()) 
        {
            antPresent=true;
            return antPresent;
        }

    }    
    return antPresent;

}



bool checkForBug(const int &X, const int &Y)
{
    bool bugPresent=false;
    for(int i=0;i<bugSize;i++)
    {
        if(X == bugVector[i].getX() && Y == bugVector[i].getY()) 
        {
            bugPresent=true;
            return bugPresent;
        }

    }    
    return bugPresent;    

}
void init()
{
    int X, Y;
    srand(time(0));
    //bugs first
    bugSize=0;
    antSize=0;
    while(bugSize<MAX_BUGS)
    {
        X = rand() % 19 + 7;
        Y = rand() % 19 + 7; 
 
        if(checkEmpty(X,Y))
        {   
            spawnBug(X,Y);
        }
    }

   


    //Ants Next
    while(antSize<MAX_ANTS)
    {
        X = rand() % 19 + 7;
        Y = rand() % 19 + 7; 
  
        if(checkEmpty(X,Y))
        {  
            spawnAnt(X,Y);
            
        }
    }


}

void displayWorld()
{
    for(int i=0;i<GRIDSIZE;i++)
    {
        for(int j=0;j<GRIDSIZE;j++)
        {
            if(checkForAnt(j,i))
            {
                cout<<"o"<<"\t";

            }
            else
            if(checkForBug(j,i))
            {
                cout<<"X"<<"\t";

            }
            else
            {
                cout<<"-"<<"\t";    
            }
            
        }
        cout<<"\n";
    }
    

}
//  common function definitions end here


void moveCritters()
{
    int targetX=INVALID_MOVE, targetY=INVALID_MOVE;
    //moving bugs first
    for(int i=0;i<bugSize;i++)
    {  
        
        bugVector[i].move(targetX, targetY);
        
        bugVector[i].checkStarve();
        bugVector[i].checkBreed();
    }

 //kill bugs marked to be killed
 killBugs();

//kill ants marked to be killed
 killAnts();


    //moving ants next 
    targetX=INVALID_MOVE, targetY=INVALID_MOVE;

    for(int i=0;i<antSize;i++)
    {
        antVector[i].move(targetX, targetY);
        antVector[i].checkBreed();
    }      

    //kill ants marked to be killed
    killAnts();


        
}


int main()
{
    antVector.reserve(GRIDSIZE*GRIDSIZE);
    bugVector.reserve(GRIDSIZE*GRIDSIZE);

    int time=0;
    string userinput;
        
    init();
    cout<<"World at time: "<<time<<" BugSize: "<<bugSize<<" AntSize: "<<antSize<< "\n";
    displayWorld();
    cout<<"Press enter to continue:";
    time++;
    while(true)
    {   
        getline(cin, userinput);
        moveCritters();
        cout<<"World at time: "<<time<<" BugSize: "<<bugSize<<" AntSize: "<<antSize<< "\n";
        displayWorld();
        cout<<"Press enter to continue:";
        time++;

    }

    return 0;
}