// delete a value from a BSTree
// - return new BSTree t without Key k

BSTree BSTreeDelete(BSTree t, Key k)
{
	if (t == NULL) return NULL;

	int diff = cmp(k,t->value);

	if (diff < 0)
		t->left = BSTreeDelete(t->left, k);
	else if (diff > 0)
		t->right = BSTreeDelete(t->right, k);
	else // (diff == 0)
		t = deleteRoot(t);
	return t;
}





// delete root of tree

BSTree deleteRoot(BSTree t)
{
	Link newRoot;
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL) {
		newRoot = t->right;
		free(t);
		return newRoot;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free(t);
		return newRoot;
	}
	else {  // (t->left != NULL && t->right != NULL)
		// so has two subtrees
		// - find inorder successor (grab value)
		// - delete inorder successor node
		// - move its value to root
		Link parent = t;
		Link succ = t->right; // not null!
		while (succ->left != NULL) {
			parent = succ;
			succ = succ->left;
		}
		int succVal = succ->value;
		t = BSTreeDelete(t,succVal);
		t->value = succVal;
		return t;
	}
}
