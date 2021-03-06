#include <iostream>
#include <cmath>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class Edge {

	private:
		struct edgelist {
			int Nobjs;
			edgelist(int Nobj_) : Nobjs(Nobj_) {
				//int Nobjs; // Number of Objects

				objLoc.resize(Nobjs);
			};

			std::string action; // Edge "label" (robot action). For head node in list, this is "none"
			std::string grpLoc; // End Effector location
			std::vector<std::string> objLoc; // Array containing locations of obj in respective locations
			int nodeind; // Node index or name
			float resource; // Weight of connecting edge, representing resource function

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

		void append(std::string action_, std::string grpLoc_, std::vector<std::string> objLoc_, int nodeind_) {
			if (isempty(head)) {
				edgelist* newNode = new edgelist(Nobj);
				//cout<<"Creating new list: "<<newNode<< endl;
				newNode->action = "none"; // Edge labels don't apply to first node in list
				newNode->grpLoc = grpLoc_;
				newNode->objLoc = objLoc_;
				newNode->nodeind = nodeind_;
				newNode->resource = -1; // Edge labels don't apply to first node in list

				head = newNode;
				prev = newNode;
				newNode->adjptr = nullptr;
			} else {
				edgelist* newNode = new edgelist(Nobj);
				//cout<<"Appending onto list: "<<newNode<< endl;
				newNode->action = action_;
				newNode->grpLoc = grpLoc_;
				newNode->objLoc = objLoc_;
				newNode->nodeind = nodeind_;
				newNode->resource = resourceFunction(action_, grpLoc_);

				prev->adjptr = newNode;
				prev = newNode;
				newNode->adjptr = nullptr;	
			}
		}

		float resourceFunction(std::string action_, std::string grpLoc_) {
			if (action_ == "grab") {
				return 5;
			} else if (action_ == "drop") {
				return 5;
			} else if (action_ == "transit") {
				return 25;
			} else if (action_ == "transfer") {
				if (grpLoc_ == "top") {
					return 75;
				} else {
					return 50;
				}
			} else {
				cout<< "Invalid action in resourceFunction" <<endl;
				return 0;
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


		int isincluded(std::string grpLoc_, std::vector<std::string> objLoc_) {
			// Two nodes are equal if the grpLoc and the objLoc's are equal
			int iseq = -1;
			for (int i=0; i<heads.size(); i++) {
				//if (heads[i]->action==action_) {
				if (heads[i]->grpLoc==grpLoc_){
					/*
					   bool arreq = true;
					   for (int ii =0; ii<Nobj; i++) {
					   if (heads[i]->objLoc[ii]!=objLoc_[ii]) {
					   arreq = false
					   }	
					   }
					   if (arreq) {
					   iseq = i;
					   break;
					   }*/
					if (heads[i]->objLoc==objLoc_) {
						iseq = i;
						break;
					}
				}
				//}
			}
			return iseq;

		}


		int returnEdgeCount() {
			return ind;
		}

		~Edge() {
			cout<< "Deconstructing " << heads.size() << " lists..."<<endl;

			for (int i=0; i<heads.size(); i++) {
				auto currptr = heads[i];
				while (currptr!=nullptr) {
					//cout<< "  Currptr = "<<currptr<<endl;
					auto nextptr = currptr->adjptr;
					//cout<< "Deleted: "<<currptr<< endl;
					delete currptr;
					currptr = nextptr;
				}
			}

		}


};




////////////////////////////////////////////////////////////////////////////////////
struct edgeV {
	std::string action; 
	std::string grpLoc; 
	std::vector<std::string> objLoc; 
	//int val; // node "v"
	//float weight; // Weight of connecting edge, this should be =0 for curr node
};


void checkadd(Edge& v, edgeV* vp, int& ind, int currnode) {
	int alreadythere;
	alreadythere = v.isincluded(vp->grpLoc, vp->objLoc);
	if (alreadythere==-1) {
		ind++;
		v.append(vp->action, vp->grpLoc, vp->objLoc, ind);
		// This creates the parent node in list
		// This adds the previous node to the list DO NOT USE FOR DIRECTIONALITY
		v.nextnode();
		cout<< "Added node: "<< ind << " to the set!" << endl;
		v.checkout(currnode);
		v.append(vp->action, vp->grpLoc, vp->objLoc, ind);
		v.nextnode();

	} else {
		if (alreadythere!=currnode) {
			v.checkout(currnode);
			v.append(vp->action, vp->grpLoc, vp->objLoc, alreadythere);
			v.nextnode();
		}
	}

}


int main () {

	int Nobj = 3;
	int Nloc = 5;
	//Edge spook(Nobj);

	std::vector<std::string> tempObjLoc;
	tempObjLoc = {"2","4","1"}; // obj 1 is in location 5, obj 2 is in location 3
	// top, leftbase, rightbase, elsewhere_1, elsewhere_2

	// append(action, grpLoc, objLoc, nodeind)
	/*
	   spook.append("hello", "1", tempObjLoc, 0);
	   spook.append("transit", "2", tempObjLoc, 1);
	   spook.nextnode();


	   spook.append("hello", "1", tempObjLoc, 0);
	   spook.append("transit", "2", tempObjLoc, 1);
	   spook.append("transit", "2", tempObjLoc, 1);
	   spook.append("transit", "2", tempObjLoc, 1);

	   spook.nextnode();
	   */


	int ind = 0;
	int currnode = 0;

	// Array containing names of all possible locations; you can use other methods to populate this array
	std::string locationlist[Nloc] = {"elsewhere1", "elsewhere2", "rightbase", "leftbase", "top"};

	std::unordered_map<std::string, bool> occupied;


	Edge v(Nobj);
	edgeV vp;
	tempObjLoc = {"rightbase","elsewhere1","elsewhere2"}; // obj 1 is in location 5, obj 2 is in location 3
	v.append("hello", "elsewhere2", tempObjLoc, 0);
	v.nextnode();
	do {
		auto cptr = v.heads[currnode];
		bool gripping = false;
		int grpObjInd;
		for (int i=0; i<Nloc; i++){
			occupied[locationlist[i]] = false;
		}

		for (int i=0; i<Nobj; i++) {
			if (cptr->objLoc[i]=="ee") {
				gripping = true;
				grpObjInd = i;
			}
			occupied[cptr->objLoc[i]] = true;
		}

		//if (cptr->grpLoc == "free") {
		if (gripping) { // TRANSFER
			for (int i=0; i<Nloc; i++) { //
				if (!occupied[locationlist[i]]) {
					vp.grpLoc = locationlist[i];
					vp.action = "transfer";
					vp.objLoc = cptr->objLoc;
					checkadd(v, &vp, ind, currnode);
				}	
			} 
			// DROP
			vp.grpLoc = cptr->grpLoc; //"free";
			vp.objLoc = cptr->objLoc; 
			vp.action = "drop";
			vp.objLoc[grpObjInd] = cptr->grpLoc;
			checkadd(v, &vp, ind, currnode);

		} else { //TRANSIT
			for (int i=0; i<Nobj; i++) {
				vp.grpLoc = cptr->objLoc[i];
				vp.action = "transit";
				vp.objLoc = cptr->objLoc;
				checkadd(v, &vp, ind, currnode);
			}
			// GRAB
			vp.grpLoc = cptr->grpLoc;
			vp.objLoc = cptr->objLoc;
			vp.action = "grab";
			for (int i=0; i<Nobj; i++) {
				if (cptr->objLoc[i] == cptr->grpLoc) {
					vp.objLoc[i] = "ee";
				}	
			}
			checkadd(v, &vp, ind, currnode);

		}
		//} else {
		/*
		vp.grpLoc = "free";
		vp.objLoc = cptr->objLoc; 
		if (gripping) {
			vp.action = "drop";
			vp.objLoc[grpObjInd] = cptr->grpLoc; 
			checkadd(v, &vp, ind, currnode);
		} else {
			vp.action = "grab";
			for (int i=0; i<Nobj; i++) {
				if (cptr->objLoc[i] == cptr->grpLoc) {
					vp.objLoc[i] = "0";
				}	
			}
			checkadd(v, &vp, ind, currnode);
		}


		//}
		*/





		currnode++;
	} while (ind>=currnode);



	cout<< v.heads.size() << " nodes."<< endl;	

	int Nalph = 0;
	for (int i=0; i<v.heads.size(); i++){
		auto adjptrm = v.heads[i];
		cout <<	"Current node index:" << adjptrm->nodeind << endl;
		cout <<	"grpLoc:" << adjptrm->grpLoc << endl;
		cout << "Objs in Loc:" << endl;
		for (int ii=0; ii<adjptrm->objLoc.size(); ii++) {
			cout << "obj:" << ii+1 << " in location:" << adjptrm->objLoc[ii] <<endl;
		}
		adjptrm = adjptrm->adjptr;
		while (adjptrm!=nullptr) {
			cout << " -Connected node index:" << adjptrm->nodeind << endl;
			cout << "     grpLoc:" << adjptrm->grpLoc << endl;
			cout << "     action label:" << adjptrm->action << endl;
			cout << "     Objs in Loc:" << endl;
			for (int ii=0; ii<adjptrm->objLoc.size(); ii++) {
				cout << "       obj:" << ii+1 << " in location:" << adjptrm->objLoc[ii] <<endl;
			}
			adjptrm = adjptrm->adjptr;
			Nalph++;
		}
	}


int Nstates = v.heads.size();
int Nobs = 0;
std::string obsarr[Nstates];
std::string statelabel[Nstates];
for (int i=0; i<Nstates; i++) {
	obsarr[i] = "    observation: '";
	//std::string tempstr3(1, v.heads[i]->action);
	//statelabel[i] = tempstr3;
	//statelabel[i].append("_");
	//statelabel[i].append(std::to_string(v.heads[i]->eeLoc));
	//statelabel[i].append("_");
	//statelabel[i].append(std::to_string(v.heads[i]->grpObj));
	//statelabel[i].append("_");
	for (int ii=0; ii<v.heads[i]->objLoc.size(); ii++) {
		obsarr[i].append("P");
		obsarr[i].append(std::to_string(ii));
		obsarr[i].append(v.heads[i]->objLoc[ii]);

		obsarr[i].append("_");
		//statelabel[i].append("P");
		//statelabel[i].append(std::to_string(ii));
		statelabel[i].append(v.heads[i]->objLoc[ii]);
		statelabel[i].append("_");
	}
	statelabel[i].append(v.heads[i]->grpLoc);
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


// PRINT FILE 
const char *filepath = "/home/peter/reactiveImp/TS_reactive.yaml";
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
		file << "    '" << statelabel[adjptrm->nodeind] << "':"<<endl;
		file << "      symbols:" <<endl;
		std::string tempstr;
		tempstr = adjptrm->action;
		//tempstr.append("_to_");
		//std::string tempstr2(1, adjptrm->action);
		//tempstr.append(tempstr2);
		//tempstr.append("_");
		//tempstr.append(to_string(v.heads[i]->grpObj));
		//tempstr.append("eeL");
		//tempstr.append(to_string(adjptrm->eeLoc));
		//tempstr.append("_");
		//tempstr.append("grp");
		//tempstr.append(to_string(adjptrm->grpObj));
		//tempstr.append("'");
		file<< "        - '" << tempstr <<"'"<<endl;
		//for (int ii=0; ii<adjptrm->objLoc.size(); ii++) {
		//cout << "       obj:" << ii+1 << " in location:" << adjptrm->objLoc[ii] <<endl;
		//}
		adjptrm = adjptrm->adjptr;
	}
}






	/*
	// Create graph M with nodes "grasp (g)", "hold (h)", "place (p)", "move (m)"
	int card_M = 4;
	int Nobj = 3;
	int card_L = 5;
	std::vector<int> nullv;
	nullv.clear();
	nullv.push_back(69);


	// CONSTRUCT GRAPH V

	char actionOPT[card_M] = {'g', 'h', 'p', 'm'};

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
		//cint Nstates = v.heads.size();
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
}out << "     Objs in Loc:" << endl;
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
*/
return 0;
}


