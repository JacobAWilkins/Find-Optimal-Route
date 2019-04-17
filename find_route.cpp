#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct node {
	char state[255];
	int depth, cost;
	struct node *parent;
};

struct succ {
	char from[255], to[255];
	int dist;
};

struct heur {
	char to[255];
	int dist;
};

// Expand the fringe
vector<struct succ> SuccFunc(int succSize, succ succArray[], char state[255]) {
	vector<struct succ> children;
	int i = 0;
	for (i = 0; i < succSize; i++) {
		if (strcmp(succArray[i].from, state) == 0) {
			struct succ temp;
			strcpy(temp.from, succArray[i].from);
			strcpy(temp.to, succArray[i].to);
			temp.dist = succArray[i].dist;
			children.push_back(temp);
		}
	}
	return children;
}

// Find min value in fringe
int MinIndex(queue<struct node *> &fringe, int sortedFringe) {
	int i, minIndex = -1, minVal = INT_MAX, n = fringe.size();
	for (int i = 0; i < n; i++) {
		struct node *current = fringe.front();
		fringe.pop();
		if (current->cost <= minVal && i <= sortedFringe) {
			minIndex = i;
			minVal = current->cost;
		}
		fringe.push(current);
	}
	return minIndex;
}

// Insert min value to end of fringe
void InsertToEnd(queue<struct node *> &fringe, int minIndex) {
	int i, n = fringe.size();
	struct node *minVal;
	for (i = 0; i < n; i++) {
		struct node *current = fringe.front();
		fringe.pop();
		if (i != minIndex) {
			fringe.push(current);
		}
		else {
			minVal = current;
		}
	}
	fringe.push(minVal);
}

// Sort Fringe
void SortFringe(queue<struct node *> &fringe) {
	int n = fringe.size();
	for (int i = 1; i <= n; i++) {
		int minIndex = MinIndex(fringe, fringe.size() - i);
		InsertToEnd(fringe, minIndex);
	}
}

// Find min value in fringe with heuristics
int MinIndexHeur(queue<struct node *> &fringe, int sortedFringe, heur heurArray[], int heurCount) {
	int i, j, minIndex = -1, minVal = INT_MAX, n = fringe.size(), totCost, heuristic;
	for (int i = 0; i < n; i++) {
		struct node *current = fringe.front();
		fringe.pop();
		
		for (j = 0; j < heurCount; j++) {
			if (strcmp(current->state, heurArray[j].to) == 0) {
				heuristic = heurArray[j].dist;
			}
		}
		totCost = current->cost + heuristic;
		
		if (totCost <= minVal && i <= sortedFringe) {
			minIndex = i;
			minVal = totCost;
		}
		fringe.push(current);
	}
	return minIndex;
}

// Sort Fringe with heuristics
void SortFringeHeur(queue<struct node *> &fringe, heur heurArray[], int heurCount) {
	int n = fringe.size();
	for (int i = 1; i <= n; i++) {
		int minIndex = MinIndexHeur(fringe, fringe.size() - i, heurArray, heurCount);
		InsertToEnd(fringe, minIndex);
	}
}

int main(int argc, char *argv[]) {
	char city1[255], city2[255], *startState = argv[2], *goalTest = argv[3];
	int dist, succCount = 0, exp = 0, heurCount = 0;
	
	// Initialize root
	struct node *root = (struct node*)malloc(sizeof(struct node));
  strcpy(root->state, startState);
	root->parent = NULL;
	root->depth = 0;
	root->cost = 0;
	
	// Create heuristic array
	if (argc == 5) {
		// Count heuristics
		char toCity[255];
		int heurDist;
		FILE *heuristic = fopen(argv[4], "r");
		while (1) {
			fscanf(heuristic, "%s %d", toCity, &heurDist);
			if (strcmp(toCity, "END") == 0) {
				break;
			}
			else {
				heurCount++;
			}
		}
		rewind(heuristic);
		fclose(heuristic);
	}
	// Create heuristics array
	heur heurArray[heurCount];
	if (argc == 5) {
		char toCity[255];
		int heurDist;
		FILE *heuristic = fopen(argv[4], "r");
		int i;
		for (i = 0; i < heurCount; i++) {
			fscanf(heuristic, "%s %d", toCity, &heurDist);
			if (strcmp(toCity, "END") == 0) {
				break;
			}
			strcpy(heurArray[i].to, toCity);
			heurArray[i].dist = heurDist;
		}
		fclose(heuristic);
	}
	
	// Count bidirectional edges
	FILE *input = fopen(argv[1], "r");
	while (1) {
		fscanf(input, "%s %s %d", city1, city2, &dist);
		if (strcmp(city1, "END") == 0) {
			break;
		}
		else {
			succCount++;
		}
	}
	int succSize = 2*succCount;
	
	// Create successor array
	succ succArray[succSize];
	rewind(input);
	int i = 0, j = 0;
	for (i = 0; i < succCount; i++) {
		fscanf(input, "%s %s %d", city1, city2, &dist);
		if (strcmp(city1, "END") == 0) {
			break;
		}
		strcpy(succArray[j].from, city1);
		strcpy(succArray[j].to, city2);
		succArray[j].dist = dist;
		j++;
		strcpy(succArray[j].from, city2);
		strcpy(succArray[j].to, city1);
		succArray[j].dist = dist;
		j++;
	}
	fclose(input);
	
	// Graph Search (uninformed or informed depending on command line arguments)
	int pathFound = 0;
	struct node *success;
	if (argc == 5) {
		// Informed Search
		vector<struct succ> children;
		vector<string> closed;
		queue<struct node *> fringe;
		fringe.push(root);
		while (fringe.size() != 0) {
			int inClosed = 0, counter = 0;
			struct node *parent;
			parent = fringe.front();
			fringe.pop();
			exp++;
			if (strcmp(parent->state, goalTest) == 0) {
				success = parent;
				pathFound = 1;
				break;
			}
			// Check if state is in closed set
			for (i = 0; i < closed.size(); i++) {
				if (parent->state == closed.at(i)) {
					inClosed = 1;
					break;
				}
			}
			if (inClosed == 1) {
				continue;
			}		
			closed.push_back(parent->state);
			children = SuccFunc(succSize, succArray, parent->state);
			for (i = 0; i < children.size(); i++) {
				struct node *temp = (struct node*)malloc(sizeof(struct node));
				strcpy(temp->state, children.at(i).to);
				temp->depth = parent->depth + 1;
				temp->cost = parent->cost + children.at(i).dist;
				temp->parent = parent;
				fringe.push(temp);
			}
			// Sort Fringe
			SortFringeHeur(fringe, heurArray, heurCount);
		}
	}
	else {
		// Uninformed Search
		vector<struct succ> children;
		vector<string> closed;
		queue<struct node *> fringe;
		fringe.push(root);
		while (fringe.size() != 0) {
			int inClosed = 0, counter = 0;
			struct node *parent;
			parent = fringe.front();
			fringe.pop();
			exp++;
			if (strcmp(parent->state, goalTest) == 0) {
				success = parent;
				pathFound = 1;
				break;
			}
			// Check if state is in closed set
			for (i = 0; i < closed.size(); i++) {
				if (parent->state == closed.at(i)) {
					inClosed = 1;
					break;
				}
			}
			if (inClosed == 1) {
				continue;
			}		
			closed.push_back(parent->state);
			children = SuccFunc(succSize, succArray, parent->state);
			for (i = 0; i < children.size(); i++) {
				struct node *temp = (struct node*)malloc(sizeof(struct node));
				strcpy(temp->state, children.at(i).to);
				temp->depth = parent->depth + 1;
				temp->cost = parent->cost + children.at(i).dist;
				temp->parent = parent;
				fringe.push(temp);
			}
			// Sort Fringe
			SortFringe(fringe);
		}
	}

	// Print number of node expanded and distance
	cout << endl << "nodes expanded: " << exp << endl;
	if (pathFound == 0) {
		cout << "distance: infinity" << endl;
		cout << "route:" << endl << "none" << endl << endl;
		return 0;
	}
	cout << "distance: " << success->cost << " km" << endl;
	
	// Calculate Route
	int depth = success->depth;
	succ route[depth];
	int routeSize = depth;
	while (depth > 0) {
		struct succ temp;
		strcpy(temp.to, success->state);
		strcpy(temp.from, success->parent->state);
		int i = 0;
		for (i = 0; i < succSize; i++) {
			if ((strcmp(succArray[i].to, temp.to) == 0) && (strcmp(succArray[i].from, temp.from) == 0)) {
				temp.dist = succArray[i].dist;
				route[depth - 1] = temp;
				depth--;
				success = success->parent;
			}
		}
	}
	
	// Print route
	cout << "route:" << endl;
	for (i = 0; i < routeSize; i++) {
		cout << route[i].from << " to " << route[i].to << ", " << route[i].dist << " km" << endl;
	}
	cout << endl;
	
	return 0;
}
