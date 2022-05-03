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

  bool condicions_wizard(Pos p){
    return cell(p).type != Abyss and cell(p).type != Granite and cell(p).type != Rock;
  }

  bool buscar_dwarf(Pos p, set<Pos> posicions){
    bool dwarf = false;
    vector<int> d = dwarves(me());
    for(const auto& id : d){
      if(unit(id).pos == p and posicions.count(p) == 0){
        dwarf = true;
        for(int i = 0; i< 8; i += 2){
          Pos p_tmp = p + Dir(i);
          if(pos_ok(p_tmp) and cell(p_tmp).id != -1 and unit(cell(p_tmp).id).type == Wizard and unit(cell(p_tmp).id).player == me()){
            dwarf = false;
            break;
          }
        }
        break;
      }
    }
    return dwarf;
  }

  Pos bfs_wizard(Pos p, vector<vector<int>>& dist, vector<vector<Pos>>& prev, set<Pos> posicions){
    dist[p.i][p.j] = 0;
    queue<Pos> Q;
    Q.push(p);
    while(not Q.empty()){
      Pos u = Q.front();
      Q.pop();
      for(int i = 0; i < 8; i+=2){
        Pos p2 = u + Dir(i);
        if(pos_ok(p2) and dist[p2.i][p2.j] == inf and condicions_wizard(p2)){
          Q.push(p2);
          dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
          prev[p2.i][p2.j] = u;
          if(buscar_dwarf(p2, posicions)) return p2;
        }
      }
    }
    return Pos(-1, -1);
  }

    bool condicions_dwarf(Pos p){
    bool trol = false;
    vector<int> t = trolls();
    for(const auto& id : t){
      if(unit(id).pos == p) {
        trol = true;
        break;
      }
    }
    if(round() > 125 or nb_treasures(me()) > 20){
      return cell(p).type != Abyss and cell(p).type != Granite and not trol and cell(p).owner != me() and cell(p).type != Rock;
    }
    else{
      return cell(p).type != Abyss and cell(p).type != Granite and not trol and cell(p).owner != me();
    }
  }

  bool dwarf_accio(Pos p2, Pos p_ini){
    bool moure = false;
    if(cell(p2).id != -1 and cell(p2).owner != me() and cell(p2).id != balrog_id() and unit(cell(p_ini).id).health >= unit(cell(p2).id).health) moure = true;
    else if(cell(p2).treasure) moure = true;
    else if((round() > 125 or nb_treasures(me()) > 15) and cell(p2).owner != me() and cell(p2).type != Outside) moure = true;
    return moure;
  }

  Pos bfs_dwarf(Pos p, vector<vector<int>>& dist, vector<vector<Pos>>& prev, set<Pos> posicions){
    dist[p.i][p.j] = 0;
    queue<Pos> Q;
    Q.push(p);
    while(not Q.empty()){
      Pos u = Q.front();
      Q.pop();
      for(int i = 0; i < 8; ++i){
        Pos p2 = u + Dir(i);
        if(pos_ok(p2) and dist[p2.i][p2.j] == inf and condicions_dwarf(p2)){
          Q.push(p2);
          dist[p2.i][p2.j] = dist[u.i][u.j] + 1;
          prev[p2.i][p2.j] = u;
          if(posicions.count(p2) == 0 and dwarf_accio(p2, p)) return p2;
        }
      }
    }
    return Pos(-1, -1);
  }

  Pos enemic_proper(Pos p){
    for(int i = 0; i<8; ++i){
      Pos p2 = p + Dir(i);
      if(pos_ok(p2)){
        if(cell(p2).id == balrog_id()){
          int i = p.i - p2.i;
          int j = p.j - p2.j;
          return Pos(p.i + i, p.j + j);
        }
        else if(cell(p2).id != -1 and unit(cell(p2).id).player != me()){
          bool trol = false;
          vector<int> t = trolls();
          for(const auto& id : t){
            if(unit(id).pos == p2){ 
              trol = true;
              break;
            }
          }
          if(not trol){
            if(unit(cell(p).id).health >= unit(cell(p2).id).health) return p2;
          }
          else{
            int i = p.i - p2.i;
            int j = p.j - p2.j;
            return Pos(p.i + i, p.j + j);
          }
        }
      }
    }
    return Pos(-1, -1);
  }

  void executar_comand(int id, int i2, int j2){
    if(i2 == 1 and j2 == 0) {
          command(id, Bottom);
        }
        else if(i2 == 1 and j2 == 1){
          command(id, BR);
        }
        else if(i2 == -1 and j2 == 0) {
          command(id, Top);
        }
        else if(i2 == -1 and j2 == -1) {
          command(id, TL);
        }
        else if(i2 == 0 and j2 == 1) {
          command(id, Right);
        }
        else if(i2 == -1 and j2 == 1){
          command(id, RT);
        }
        else if(i2 == 0 and j2 == -1) {
          command(id, Left);
        }
        else if(i2 == 1 and j2 == -1) {
          command(id, LB);
        }
  }

typedef pair<double, Pos> ArcP;

void dijkstra_eda(Pos s, vector<vector<int>> &d, vector<vector<Pos>> &p) {
  d = vector<vector<int>>(60, vector<int>(60, inf));
  d[s.i][s.j] = 0;
  p = vector<vector<Pos>>(60, vector<Pos>(60, Pos(-1, -1)));;
  vector<vector<bool>> S(60, vector<bool>(60, false));
  priority_queue<ArcP, vector<ArcP>, greater<ArcP> > Q;
  Q.push(ArcP(0, s));
  while (not Q.empty()) {
    Pos u = Q.top().second;
    Q.pop();
    if (not S[u.i][u.j]) {
      S[u.i][u.j] = true;
      for (int i = 0; i < 8; ++i) {
        Pos v = u + Dir(0);
        if(pos_ok(v)){
          if(cell(v).type != Granite and cell(v).type != Abyss){
            int c;
            if(cell(v).type == Rock) c = cell(v).turns;
            else c = 1;
            if (d[v.i][v.j] > d[u.i][u.j] + c) {
                d[v.i][v.j] = d[u.i][u.j] + c;
                p[v.i][v.j] = u;
                Q.push(ArcP(d[v.i][v.j], v));
            }
          }
        }
      }
    }
  }
}

  void moure_dwarf(){
    vector<int> d = dwarves(me());
    set<Pos> posicions;
    for(auto& id : d){
      Pos p_tmp = enemic_proper(unit(id).pos);
      vector<vector<int>> dist(60, vector<int>(60,inf));
      vector<vector<Pos>> prev(60, vector<Pos>(60, Pos(-1, -1)));
      Pos p = bfs_dwarf(unit(id).pos, dist, prev, posicions);
      if(p_tmp != Pos(-1, -1)){
        int i = p_tmp.i - unit(id).pos.i;
        int j = p_tmp.j - unit(id).pos.j;
        executar_comand(id, i ,j);
      }
      else if(p != Pos(-1, -1)){
        posicions.insert(p);
        int i = p.i;
        int j = p.j;
        while(dist[i][j] > 1){
          Pos p_pre = prev[i][j];
          i = p_pre.i;
          j = p_pre.j;
        }
        int i2 = i - unit(id).pos.i;
        int j2 = j - unit(id).pos.j;
        executar_comand(id, i2, j2);
      }
      else command(id, None);
    }
  }

  bool dwarf_a_prop(Pos p){
    for(int i = 0; i < 8; i += 2){
      Pos p_tmp = p + Dir(i);
      if(pos_ok(p_tmp)){
        return cell(p_tmp).id != -1 and unit(cell(p_tmp).id).type == Dwarf and unit(cell(p_tmp).id).player == me();
      }
    }
    return false;
  }

  void moure_wizard(){
    vector<int> w = wizards(me());
    set<Pos> posicions;
    for(auto& id : w){
      vector<vector<int>> dist(60, vector<int>(60,inf));
      vector<vector<Pos>> prev(60, vector<Pos>(60, Pos(-1, -1)));
      Pos p = bfs_wizard(unit(id).pos, dist, prev, posicions);
      posicions.insert(p);
      if (p != Pos(-1, -1) and not dwarf_a_prop(unit(id).pos)){
        int i = p.i;
        int j = p.j;
        while(dist[i][j] > 1){
          Pos p_pre = prev[i][j];
          i = p_pre.i;
          j = p_pre.j;
        }
        int i2 = i - unit(id).pos.i;
        int j2 = j - unit(id).pos.j;
        executar_comand(id, i2, j2);
      }
      else {
        command(id, None);
      }
    }
  }

  virtual void play () {
    moure_dwarf();
    moure_wizard();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);