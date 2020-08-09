#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// Linked List cheerios 
// ALWAYS INITIALIZE TO NULL POINTER
class Edge {

	private:
		struct edgelist {
			int Nobj; // Number of Objects
			char action; // End effector action
			int eeLoc; // End effector location
			int grpObj; // Object being manipuled
			std::vector<int> objLoc; // Vector containing locations of obj in respective 
			// positions
			edgelist(int Nobj_) : Nobj(Nobj_) {
				//objLoc.resize(Nobj);
			};

			int val; // node "v"
			float weight; // Weight of connecting edge, this should be =0 for curr node
			std::string label;
			edgelist* adjptr;
		};

		bool checking = false;

		int ind, indc;
		int Nobj;

		edgelist* head;
		edgelist* prev;

		std::vector<edgelist*> delvec;
		std::vector<edgelist*> prevs;
	public:
		edgelist* adjptrm;
		std::vector<edgelist*> heads;
		Edge(int Nobj_) : Nobj(Nobj_) {
			ind = 0;
			head = nullptr;
			prev = nullptr;

		}

		bool isempty(edgelist* head) {
			if (head==nullptr) {
				return true;
			} else {
				return false;
			}
		}

		//void append(edgelist*& head, edgelist*& prev, int nodeVal, float nodeWeight) {
		void append(char action_, int eeLoc_, int grpObj_, std::vector<int> objLoc_, int val_, float weight_) {
			if (isempty(head)) {
				edgelist* newNode = new edgelist(Nobj);

				newNode->action = action_;
				newNode->eeLoc = eeLoc_;
				newNode->grpObj = grpObj_;
				newNode->objLoc = objLoc_;
				newNode->val = val_;
				newNode->weight = weight_;

				head = newNode;
				prev = newNode;
				newNode->adjptr = nullptr;
				delvecapp(delvec, newNode);
			} else {
				edgelist* newNode = new edgelist(Nobj);

				newNode->action = action_;
				newNode->eeLoc = eeLoc_;
				newNode->grpObj = grpObj_;
				newNode->objLoc = objLoc_;
				newNode->val = val_;
				newNode->weight = weight_;

				prev->adjptr = newNode;
				prev = newNode;
				newNode->adjptr = nullptr;	
				delvecapp(delvec, newNode);
			}
		}

		void checkout(int indnewc) {
			indc = indnewc;
			if (isempty(head)) {
				head = heads[indc];
				prev = prevs[indc];	
				checking = true;
			} else {
				cout << "Cannot checkout, working list is not empty" <<endl;
			}

		}

		void nextnode(){
			if (checking) {
				//cout<< "checking head:"<<heads[indc]<< ", reset head:"<<head<<endl;
				if (heads[indc] == head) {
					heads[indc] = head;
					prevs[indc] = prev; 
					// reset the pointer keeping track of last node in list
					checking = false;
				} else {
					cout<<"HEADS ARE A MISMATCH"<<endl;
				}
			} else {
				heads.push_back(head);
				prevs.push_back(prev);
				ind++;
			}
			head = nullptr;
			prev = nullptr;


		}


		int isequal(char action_, int eeLoc_, int grpObj_, std::vector<int> objLoc_) {
			int iseq = -1;
			for (int i=0; i<heads.size(); i++) {
				if (heads[i]->action==action_) {
					if (heads[i]->eeLoc==eeLoc_){
						if (heads[i]->grpObj==grpObj_) {
							if (heads[i]->objLoc==objLoc_) {
								iseq = i;
								break;
							}
						}
					}
				}
			}
			return iseq;

		}

		void delvecapp(std::vector<edgelist*>& delvec, edgelist* ptr) {
			delvec.push_back(ptr);
		}

		int returnEdgeCount() {
			return ind;
		}

		~Edge() {
			cout<< "ya" << delvec.size() <<endl;
			int a = delvec.size();
			for (int i=0; i<a; i++){
				//cout<<"Deleted (delvec): "<< delvec[i] <<endl;
				delete delvec[i];
			}
			int b = heads.size();	
			for (int i=0; i<b; i++){
				//cout<<"Deleted (heads): "<< heads[i] <<endl;
				delete heads[i];
			}
			//cout<< "Deleted (temphead): "<< head <<endl;
			delete head; // i dont need this but whatever
			//cout<< "Deleted (tempprev): "<< prev <<endl;
			delete prev;
			//cout<< "HEY THER BUDY" <<endl;
			cout<< "    NA"<<endl;
		}


};




////////////////////////////////////////////////////////////////////////////////////
struct edgeV {
	int Nobj; 
	char action; 
	int eeLoc; 
	int grpObj; 
	std::vector<int> objLoc; 
	//int val; // node "v"
	//float weight; // Weight of connecting edge, this should be =0 for curr node
};





int main () {
	// Create graph M with nodes "grasp (g)", "hold (h)", "place (p)", "move (m)"
	int card_M = 4;
	int Nobj = 3;
	int card_L = 5;
	std::vector<int> nullv;
	nullv.clear();
	nullv.push_back(69);

	/*
	Edge edgeM(Nobj);
	// Syntax: append(...) currentnode->connectednode->connectednode->etc.
	// append(char action_, int eeLoc_, int grpObj_, std::vector<int> objLoc_, 
	// int val_, float weight_)
	edgeM.append('g', 0, 0, nullv, 0, 0);
	edgeM.append('h', 0, 0, nullv, 0, 1);	
	edgeM.nextnode();
	edgeM.append('h', 0, 0, nullv, 0, 0);
	edgeM.append('h', 0, 0, nullv, 0, 1);
	edgeM.append('p', 0, 0, nullv, 0, 1);
	edgeM.nextnode();
	edgeM.append('p', 0, 0, nullv, 0, 0);
	edgeM.append('m', 0, 0, nullv, 0, 1);
	edgeM.nextnode();	
	edgeM.append('m', 0, 0, nullv, 0, 0);
	edgeM.append('m', 0, 0, nullv, 0, 1);
	edgeM.append('g', 0, 0, nullv, 0, 1);
	edgeM.nextnode();
	*/
	/*
	   for (int i=0; i<edgeM.heads.size(); i++){
	   edgeM.adjptrm = edgeM.heads[i];
	//cout<< edgeM.heads[0];
	cout <<	"Current node action:" <<edgeM.adjptrm->action << endl;
	//cout << "scoob" <<edgeM.adjptrm <<endl;
	edgeM.adjptrm = edgeM.adjptrm->adjptr;
	while (edgeM.adjptrm!=nullptr) {
	//cout <<"ptrb4:"<<adjptrm<<endl;
	cout << "  Connected node action:" << edgeM.adjptrm->action << endl;
	cout << "     Weight:" << edgeM.adjptrm->weight << endl;
	cout << "     Objs in Loc:" << edgeM.adjptrm->objLoc.size() << endl;
	edgeM.adjptrm = edgeM.adjptrm->adjptr;
	//cout <<"ptrafter:"<<adjptrm<<endl;
	}
	}
	*/




	// Create graph L with "node 0" being the intermediate area, all other nodes are 
	// locations with 1-start indexing. This creates the location graph shown in the 
	// paper, but this section can be edited to represent a more complex location
	// scenario. The node val represents the index in "heads".
	
	/*
	Edge L(Nobj);
	L.append('\0', 0, 0, nullv, 0, 0);
	for (int i=1; i<(card_L+1); i++) {
		L.append('\0', i, 0, nullv, i, 1);	
	}
	L.nextnode();
	for (int i=1; i<(card_L+1); i++) {
		L.append('\0', i, 0, nullv, i, 0);
		L.append('\0', 0, 0, nullv, 0, 1);
		L.nextnode();
	}

	// Create set of objects "O" (no object is represented as "0", other objects are 1-start
	// indexing
	int set_O[Nobj+1];	
	for (int i=0; i<(Nobj+1); i++) {
		set_O[i] = i;
	}
	*/

	// CONSTRUCT GRAPH V

	char actionOPT[card_M] = {'g', 'h', 'p', 'm'};
	/*
	   int eeLocOPT[card_L+1];
	   for (int i=0; i<(card_L+1); i++) {
	   eeLocOPT[i] = i;
	   }
	   int objLocOPT[][Nobj+1];
	   for (int i=0; i<
	   */

	// append(char action_, int eeLoc_, int grpObj_, std::vector<int> objLoc_, 
	// int val_, float weight_)

	Edge v(Nobj);
	// Initializers
	edgeV vp;	
	int ind = 0;
	int currnode = 0;
	bool done = false;
	std::vector<int> tempObjLoc;
	tempObjLoc = {2,4,1}; // obj 1 is in location 5, obj 2 is in location 3
	//tempObjLoc = {2};
	// top, leftbase, rightbase, elsewhere_1, elsewhere_2
	v.append('m', 0, 0, tempObjLoc, 0, 0);
	v.nextnode();
	if (tempObjLoc.size()!=Nobj) {
		cout<< "INITIAL OBJLOC VECTOR DOES NOT MATCH NUMBER OF OBJECTS" <<endl;
	}
	do {
		auto cptr = v.heads[currnode];
		//cout<< cptr->action << endl;
		switch (cptr->action) {
			case 'g':
				if (cptr->grpObj==0) {
					//cout<< "scooby1" <<endl;
					for (int i=0; i<Nobj; i++) {
						if (cptr->eeLoc==cptr->objLoc[i]) {
							//cout<< "scooby2"<<endl;
							vp.grpObj = i+1; 
							//set vp grp object to be the
							//object being looked at
							vp.action = 'h';
							vp.eeLoc = cptr->objLoc[i];
							tempObjLoc = cptr->objLoc;
							tempObjLoc[i] = 0;
							vp.objLoc = tempObjLoc; //tempObjLoc;
							//vp.objLoc = cptr->objLoc; //tempObjLoc;

							// Search the nodes:
							int alreadythere;
							alreadythere = v.isequal(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc);
							 
							if (alreadythere==-1) {
								ind++;
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								// This creates the parent node in list
								//v.append(cptr->action, cptr->eeLoc, cptr->grpObj, cptr->objLoc, currnode, 0);
								// This adds the previous node to the list DO NOT USE FOR DIRECTIONALITY
								v.nextnode();
								cout<< "Added node: "<< ind << " to the set!" << endl;
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								v.nextnode();

							} else {
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, alreadythere, 0);
								v.nextnode();

							}
						}
					}
				}
				break;
			case 'p':{
				// Check if the object locations are free
				bool isfree = true;
				for (int i=0; i<Nobj; i++) {
					if (cptr->objLoc[i] == cptr->eeLoc) {
						isfree = false;
						break;
					}
				}
				vp.action = 'm';
				vp.eeLoc = cptr->eeLoc;
				vp.grpObj = 0;
				vp.objLoc = cptr->objLoc;
				if (isfree) {
					int placeobj = cptr->grpObj-1;
					vp.objLoc[placeobj] = cptr->eeLoc;
					int alreadythere;
							alreadythere = v.isequal(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc);
							 
							if (alreadythere==-1) {
								ind++;
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								// This creates the parent node in list
								//v.append(cptr->action, cptr->eeLoc, cptr->grpObj, cptr->objLoc, currnode, 0);
								// This adds the previous node to the list DO NOT USE FOR DIRECTIONALITY
								v.nextnode();
								cout<< "Added node: "<< ind << " to the set!" << endl;
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								v.nextnode();

							} else {
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, alreadythere, 0);
								v.nextnode();

							}

				}
				break;
				}

			default:
				char act1, act2;
				if (cptr->action == 'h') {
					act1 = 'h';
					act2 = 'p'; // case 'h':
				} else if (cptr->action == 'm') {
					act1 = 'm';
					act2 = 'g'; // case 'm':
				} 
				// Search through L heads to find current node on L graph
				//bool locvisit[cart_L+1];
				//for (int i=0; i<(cart_L+1); i++) {
				//	locvisit[i] = false;
				//}
				vp.grpObj = cptr->grpObj;
				vp.objLoc = cptr->objLoc;
				if (cptr->eeLoc!=0) {
					int alreadythere;
					int dualcount = 0;
					do {
						if ((dualcount==1)) {
							vp.action = act2;
							vp.eeLoc = cptr->eeLoc;
							//if (cptr->grpObj!=0){
							//	vp.objLoc[cptr->grpObj-1] = cptr->eeLoc;
							//}
						} else {
							vp.action = act1;
							vp.eeLoc = 0;
							//if (cptr->grpObj!=0){
							//	vp.objLoc[cptr->grpObj-1] = 0;
							//}
						}
							alreadythere = v.isequal(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc);
							 
							if (alreadythere==-1) {
								ind++;
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								// This creates the parent node in list
								//v.append(cptr->action, cptr->eeLoc, cptr->grpObj, cptr->objLoc, currnode, 0);
								// This adds the previous node to the list DO NOT USE FOR DIRECTIONALITY
								v.nextnode();
								cout<< "Added node: "<< ind << " to the set!" << endl;
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								v.nextnode();

							} else {
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, alreadythere, 0);
								v.nextnode();

							}
					
						dualcount++;
					} while (dualcount<=1);


				} else {
					for (int i=1; i<(card_L+1); i++) {
						if (i!=cptr->eeLoc) {
							vp.action = act1;
							vp.eeLoc = i;
							//if (cptr->grpObj!=0){
							//	vp.objLoc[cptr->grpObj-1] = i;
							//}
							// Search the nodes:
							int alreadythere;
							alreadythere = v.isequal(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc);
							 
							if (alreadythere==-1) {
								ind++;
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								// This creates the parent node in list
								//v.append(cptr->action, cptr->eeLoc, cptr->grpObj, cptr->objLoc, currnode, 0);
								// This adds the previous node to the list DO NOT USE FOR DIRECTIONALITY
								v.nextnode();
								cout<< "Added node: "<< ind << " to the set!" << endl;
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, ind, 0);
								v.nextnode();

							} else {
								v.checkout(currnode);
								v.append(vp.action, vp.eeLoc, vp.grpObj, vp.objLoc, alreadythere, 0);
								v.nextnode();

							}


						}
					}
				}
				/*
				// Propagate though L graph, add nodes that are found
				auto adjptrL = L.heads[currnodeL];
				while (adjptrL != nullptr) {
				if (!locvisit[currnodeL]) {

				}
				}
				*/


		}
		cout<< "Number of nodes: "<< ind+1 << ", Current node (1start): "<< currnode+1 <<endl;
		currnode++;
	} while (ind>=currnode);

	cout<< v.heads.size() << " nodes."<< endl;	

	int Nalph = 0;
	for (int i=0; i<v.heads.size(); i++){
		auto adjptrm = v.heads[i];
		//cout <<	"Current node action:" <<adjptrm->action << ", Current node name:" << adjptrm->val << endl;
		adjptrm = adjptrm->adjptr;
		while (adjptrm!=nullptr) {
			//cout << " -Connected node name:" << adjptrm->val << endl;
			//cout << "     action:" << adjptrm->action << endl;
			//cout << "     eeLoc:" << adjptrm->eeLoc << endl;
			//cout << "     grpObj:" << adjptrm->grpObj << endl;
			//cout << "     Objs in Loc:" << endl;
			for (int ii=0; ii<adjptrm->objLoc.size(); ii++) {
				//cout << "       obj:" << ii+1 << " in location:" << adjptrm->objLoc[ii] <<endl;
			}
			adjptrm = adjptrm->adjptr;
			Nalph++;
		}
	}

	// LABEL GRAPH FOR NICK
	int Nstates = v.heads.size();
	int Nobs = 0;
	std::string obsarr[Nstates];
	std::string statelabel[Nstates];
	for (int i=0; i<Nstates; i++) {
		obsarr[i] = "    observation: '";
		std::string tempstr3(1, v.heads[i]->action);
		statelabel[i] = tempstr3;
		statelabel[i].append("_");
		statelabel[i].append(std::to_string(v.heads[i]->eeLoc));
		statelabel[i].append("_");
		statelabel[i].append(std::to_string(v.heads[i]->grpObj));
		statelabel[i].append("_");
		for (int ii=0; ii<v.heads[i]->objLoc.size(); ii++) {
			obsarr[i].append("P");
			obsarr[i].append(std::to_string(ii));
			obsarr[i].append(std::to_string(v.heads[i]->objLoc[ii]));
			//obsarr[i].append("_");
			//statelabel[i].append("P");
			//statelabel[i].append(std::to_string(ii));
			statelabel[i].append(std::to_string(v.heads[i]->objLoc[ii]));
			statelabel[i].append("_");
		}
		//obsarr[i].pop_back();
		//obsarr[i].pop_back();
		obsarr[i].append("'");
		//cout<< obsarr[i] <<endl;
		bool isin = false;
		for (int ii=0; ii<i; ii++) {
			if (obsarr[i] == obsarr[ii]) {
				isin = true;
			}
		}
		if (!isin) {
			Nobs++;
		}
	}

	// PRINT FILE FOR NICK
	const char *filepath = "/home/peter/wombatsPanda/FlexfringeInvestigations/wombats/config/TS_manipulator.yaml";
	ofstream file(filepath);
	//file.open("TS_manipulator.yaml");
	
	file<< "alphabet_size: "<< Nalph <<endl;
	file<< "num_states: " << Nstates <<endl;
	file<< "num_obs: " << Nobs <<endl;
	file<< "start_state: '"<< statelabel[0] <<"'" <<endl;
	file<< "nodes:" <<endl;
	for (int i=0; i<Nstates; i++) {
		file<< "  '" << statelabel[i] <<"':" <<endl;
		file<< obsarr[i] <<endl;
	}
	file<< "edges:" <<endl;	
	for (int i=0; i<v.heads.size(); i++){
		auto adjptrm = v.heads[i];
		file <<"  '"<< statelabel[i] <<"':"<< endl;
		adjptrm = adjptrm->adjptr;
		while (adjptrm!=nullptr) {
			file << "    '" << statelabel[adjptrm->val] << "':"<<endl;
			file << "      symbols:" <<endl;
			std::string tempstr;
			tempstr = v.heads[i]->action;
			tempstr.append("_to_");
			std::string tempstr2(1, adjptrm->action);
			tempstr.append(tempstr2);
			tempstr.append("_");
			//tempstr.append(to_string(v.heads[i]->grpObj));
			tempstr.append("eeL");
			tempstr.append(to_string(adjptrm->eeLoc));
			tempstr.append("_");
			tempstr.append("grp");
			tempstr.append(to_string(adjptrm->grpObj));
			tempstr.append("'");
			file<< "        - '" << tempstr <<endl;
			//for (int ii=0; ii<adjptrm->objLoc.size(); ii++) {
				//cout << "       obj:" << ii+1 << " in location:" << adjptrm->objLoc[ii] <<endl;
			//}
			adjptrm = adjptrm->adjptr;
		}
	}





	file.close();

	return 0;
}


