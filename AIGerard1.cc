#include "Player.hh"
#include <limits>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Gerard1


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  const int inf = numeric_limits<int>::max();
  
  bool condicions_dwarf(Pos p){
    return true;
  }

  bool condicions_wizard(Pos p){
    return true;
  }

  void bfs(Pos p, int ut, vector<vector<int>>& dist, vector<vector<Pos>> prev){
    dist[p.i][p.j] = 0;

    queue<Pos> Q;
    Q.push(p);

    while(not Q.empty()){
      Pos u = Q.front();
      Q.pop();
      //Si ut==0 vol dir que és un Dwarf en cas contrari és un Wizard.
      if(ut==0){
        for(int i = 0; i < 8; ++i){
          Pos p2 = u + Dir(i);
          if(dist[p2.i][p2.j] == inf and pos_ok(p2) and condicions_dwarf(p2)){
            Q.push(p2);
            dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
            prev[p2.i][p2.j] = u;
            if(cell(p2).treasure) return;
          }
        }
      }
      else{
        for(int i = 0; i < 8; i+=2){
          Pos p2 = u + Dir(i);
          if(dist[p2.i][p2.j] == inf and pos_ok(p2) and condicions_wizard(p2)){
            Q.push(p2);
            dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
            prev[p2.i][p2.j] = u;
            if(cell(p2).treasure) return;
          }
        }
      }
    }
    return;
  }

  void moure_dwarf(){
    vector<int> d = dwarves(me());
    for(auto& i : d){
      vector<vector<int>> dist(60, vector<int>(60,inf));
      vector<vector<Pos>> prev(60, vector<Pos>(60, Pos(-1, -1)));
    }
  }

  virtual void play () {
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
