#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int helper(Node *root);


bool equalPaths(Node * root)
{
  // Add your code below
  
  if(root == NULL){

    return true;
    
  }

  return helper(root);

}


int helper(Node * root){

  int x = 0;
  int y = 0;
	int temp = 0;

  if(root->left != NULL && root->right == NULL){

		temp = helper(root->left);

		if(temp == 0){
			return 0;
		}

    x += temp;
    return x+1;

  } else if(root->right != NULL && root->left == NULL){

		temp = helper(root->right);

		if(temp == 0){
			return 0;
		}

    x += temp;
    return x+1;
   
  } else if(root->right == NULL && root->left == NULL){

    return 1;

  }

	temp = helper(root->left);

	if(temp == 0){
		return 0;
	}

  x += temp; // x == 

	temp = helper(root->right);

	if(temp == 0){
		return 0;
	}

  y += temp; // y == 
  
  if(x == y){

    return x+1;

  }

  return 0;

}
