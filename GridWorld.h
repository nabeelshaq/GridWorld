#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;


class GridWorld {

  private:
    struct Node {
        int data;
        struct Node *next;
        struct Node *prev;
    };
    
    class linkedList {
        public:
            linkedList ( ) {
                front = NULL;
                back = NULL;
            }
        
        vector<int>* list_to_vector ( ) const {
            Node *temp = front;
            vector<int> *vec = new vector<int>;
            while (temp !=  NULL) {
                (*vec).push_back(temp->data);
                temp = temp->next;
            }
            return vec;
        }
        
        int get_front(){
            return front->data;
        }
        
        Node* get_back (){
            return back;
        }
        
        int get_population(){
            return population;
        }
        
        bool empty(){
            bool returnval = true;
            if (!front){ returnval = true;}
            else{ returnval = false; }
            return returnval;
        }
        
        void push_back(int x){
            Node *tmp;
            tmp = new Node;
            tmp->data = x;
            tmp->next =  NULL;
            tmp->prev = back;
            if (front ==  NULL){ front = tmp;}
            else { back->next = tmp;}
            back = tmp;
            population++;
        }
        
        void pop_front(){
            Node* tmp = front;
            if (front != NULL) {
                tmp = front->next;
                if (tmp != NULL) { tmp->prev == NULL;}
                front = NULL;
                front = tmp;
                population--;
            }
        }

        void delete_node(Node* p){
            Node* prev = p->prev;
            Node* next = p->next;
            if (prev == NULL){
                front = NULL;
                front = next;
                front->prev = NULL;
            }
            else if (next == NULL){
                back = NULL;
                back = prev;
                back->next = NULL;
            }
            else {
                p = NULL;
                prev->next = next;
                next->prev = prev;
            }
            population--;
        }
        void empty_list() {
            Node* tmp = front;
            while (tmp != NULL) {
                Node* next = tmp->next;
                delete tmp;
                tmp = next;
            }
        }
        
    private:
        Node *front;
        Node *back;
        int population = 0;
    };
    
    
    struct person {
        int row;
        int column;
        int id;
        bool dead;
        Node *ptr;
    };
        
    int rows;
    int cols;
    int populate =0;
    int counter =0;
    linkedList **grid;
    linkedList deadpool;
    vector<person> district;

  public:
    /**
    * constructor:  initializes a "world" with nrows and
    *    ncols (nrows*ncols districtcs) in which all 
    *    districtricts are empty (a wasteland!).
    */
    GridWorld(unsigned nrows, unsigned ncols)   {
         rows = nrows;
         cols = ncols;
        grid = new linkedList*[nrows];
        int i=0;
        while (i<rows){
            grid[i] = new linkedList[ncols];
            i++;
        }
    }
    ~GridWorld(){
        int i=0,j=0;
        while(i< rows){
            while(j< cols){
                grid[i][j].empty_list();
                i++;
                j++;
            }
        }
        delete grid;
    }

    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     */
    bool birth(int row, int col, int &id){
        while ( rows <= row ||  cols <= col) {return false;}
        if (deadpool.empty() != false){
            id = counter;
            grid[row][col].push_back(id);
            district.push_back(person());
            district[counter].column = col;
            district[counter].row = row;
            district[counter].id = id;
            district[counter].dead = false;
            district[counter].ptr=grid[row][col].get_back();
            populate++;
            counter++;
        }
        else{
            id = deadpool.get_front();
            deadpool.pop_front();
            grid[row][col].push_back(id);
            district[counter].column = col;
            district[counter].row = row;
            district[counter].id = id;
            district[counter].dead = false;
            district[counter].ptr=grid[row][col].get_back();
            populate++;
        }
        return true;
    }

    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    bool death(int personID){
        while(  counter <= personID){ return false;}
        while(district[personID].dead != false) { return false;}
        grid [district[personID].row] [district[personID].column].delete_node(district[personID].ptr);
        district[personID].dead = true;
        deadpool.push_back(personID);
        populate--;
        return true;
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     */
    bool whereis(int id, int &row, int &col)const{
        while (  counter <= id) {return false; }
        while (district[id].dead != false) {return false; }
        col = district[id].column;
        row = district[id].row;
        return true;
    }

    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    bool move(int id, int targetRow, int targetCol){
        while (  counter <= id ){ return false;}
        while (district[id].dead != false) {return false;}
        while ( targetCol >=  cols || targetRow >=  rows){ return false;}
        grid[district[id].row] [district[id].column].delete_node(district[id].ptr);
        grid[targetRow][targetCol].push_back(id);
        district[id].column = targetCol;
        district[id].row = targetRow;
        district[id].ptr = grid[targetRow][targetCol].get_back();
        return true;
    }

    std::vector<int> * members(int row, int col)const{
        vector<int> *empty = new vector<int>;
        vector<int> *district = new vector<int>;
        while ( col >= cols || row >= rows ) {return empty;}
        district = grid[row][col].list_to_vector();
        return district;
    }

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     */
    int population()const{
      return populate;
    }
    
    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     */
    int population(int row, int col)const{
        while ( col > cols ||  row >= rows) {return 0;}
        int pop = grid[row][col].get_population();
        return pop;
    }
    /*
     * description:  returns number of rows in world
     */
    int num_rows()const {
      return rows;
    }/*
     * description:  returns number of columns in world
     */
    int num_cols()const {
      return cols;
    }

};

#endif
