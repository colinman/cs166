package rmq;

import java.util.*;
/**
 * An &lt;O(n), O(1)&gt; implementation of the Fischer-Heun RMQ data structure.
 *
 * You will implement this class for problem 3.iv of Problem Set One.
 */
public class FischerHeunRMQ implements RMQ {

	int b;
	float[] block_mins;
	int[] cartesianCodes;
	SparseTableRMQ blockRMQ;
	TreeNode[] cartesianTrees;
	Stack<Float> cartesianCodeStack;
	Stack<TreeNode> cartesianTreeStack;
	float[] myelems;

	class TreeNode{
		TreeNode(int idx, float v, TreeNode left, TreeNode right){
			index = idx;	
			value = v;
			leftChild = left;
			rightChild = right;
		}

		public int index;
		public float value;
		public TreeNode leftChild;
		public TreeNode rightChild;
	}	

    /**
     * Creates a new FischerHeunRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public FischerHeunRMQ(float[] elems) {
		cartesianCodeStack = new Stack<>();
		cartesianTreeStack = new Stack<>();
		this.myelems = elems;
		int n = elems.length;
		b = (int)Math.floor(Math.log(n)/Math.log(2)) + 1;
		cartesianTrees = new TreeNode[(int)Math.pow(4, b)];

      	int num_mins = (int)Math.ceil(n / b);
     	block_mins = new float [num_mins];
		cartesianCodes = new int [num_mins];

		/* From the hybrid structure */
		for (int i = 0; i < num_mins; i++) {
        	int min_idx = -1;
       		int end = (i*b + b > n) ? n : i*b + b;
        	for (int j = i*b; j < end; j++) {
        		if (min_idx == -1 || elems[j] < elems[min_idx]) min_idx = j;
        	}
        	block_mins[i] = elems[min_idx];

			/* Preprocess Cartesian trees */
			int cartesian_code = getCartesianCode(i*b, end, elems);
			if (cartesianTrees[cartesian_code] == null){
				cartesianTrees[cartesian_code] = buildCartesianTree(i*b, end, elems);
			}
			cartesianCodes[i] = cartesian_code;
      	}

   		blockRMQ = new SparseTableRMQ (block_mins);
	}

	private int codeStackPop(int code){
		code <<= 1;
		code |= 0;
		cartesianCodeStack.pop();
		return code;
	}

	private int codeStackPush(int code, float elem){
		code <<= 1;
		code |= 1;
		cartesianCodeStack.push(elem);
		return code;
	}

	private int getCartesianCode(int start, int end, float[] elems){
		int code = 0;
		for (int i = start; i < end; i++){
			while (!cartesianCodeStack.empty() 
					&& elems[i] < cartesianCodeStack.peek()){
				code = codeStackPop(code);	
			}
			code = codeStackPush(code, elems[i]);
		} 
		/* Clear the stack and finish the code */
		while(!cartesianCodeStack.empty()){
			code = codeStackPop(code);
		}
		return code;	
	}

	private TreeNode buildCartesianTree(int start, int end, float[] elems){
		TreeNode lastNodePopped = null;
		for (int i = start; i < end; i++){
			while (!cartesianTreeStack.empty() 
					&& elems[i] < ((TreeNode)cartesianTreeStack.peek()).value){
				lastNodePopped = (TreeNode)cartesianTreeStack.pop();
			}
			TreeNode newNode = new TreeNode(i - start, elems[i], lastNodePopped, null);
			lastNodePopped = null;
	
			if (!cartesianTreeStack.empty()){
				((TreeNode)cartesianTreeStack.peek()).rightChild = newNode;	
			}	
			cartesianTreeStack.push(newNode);		
		}
		/* Clear the stack */
		while(!cartesianTreeStack.empty()){
			lastNodePopped = (TreeNode)cartesianTreeStack.pop();
		}
		return lastNodePopped; // Root node!	
	}

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {
		int block1 = (int)Math.floor(i / b);
		int block2 = (int)Math.floor(j / b);
		System.out.println(b + "sized-blocks: " + block1 + " " + block2);
		int minBlock = blockRMQ.rmq(block1 + 1, block2 - 1);

	    int min1_idx = cartesianMin(block1, i % b, Math.min(j, (block1 + 1) * b) % b);
      	int min2_idx = cartesianMin(block2, Math.max(i,block2 * b) % b, j % b);

		int min_idx = minBlock;
      	if (min_idx == -1 || min1_idx != -1 && myelems[min1_idx] < myelems[min_idx]) 
			min_idx = min1_idx;
      	if (min_idx == -1 || min2_idx != -1 && myelems[min2_idx] < myelems[min_idx]) 
			min_idx = min2_idx;

      	return min_idx;        
	}

	private int cartesianMin(int block, int i, int j){
		System.out.println("Block index: " + block + "   length: " + cartesianCodes.length);	
		int code_index = cartesianCodes[block];	
		TreeNode curr = cartesianTrees[code_index];
		while (curr.index < i || curr.index > j){
//			System.out.println(curr.value);
//			System.out.println(curr.value < i);
			curr = (curr.index < i) ? curr.rightChild : curr.leftChild; 	
			if (curr == null) System.out.println("Guys I found the problem");
		}
		return block * b + curr.index;
	}
}
