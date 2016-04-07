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
  SparseTableRMQ[] blockRMQArray;
  Stack<Float> cartesianCodeStack;
  float[] elems;
			
  /**
   * Creates a new FischerHeunRMQ structure to answer queries about the
   * array given by elems.
   *
   * @elems The array over which RMQ should be computed.
   */
  public FischerHeunRMQ(float[] elems) {
    cartesianCodeStack = new Stack<>();
    this.elems = elems;
    int n = elems.length;
    // b = (int)Math.floor(Math.log(n)/Math.log(2)) + 1;
    b = (int)Math.floor(Math.log(n)) + 1;
    blockRMQArray = new SparseTableRMQ[(int)Math.pow(4, b)];

    int num_mins = (int)Math.ceil(n / (double)b);
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
      if (blockRMQArray[cartesian_code] == null){
        blockRMQArray[cartesian_code] = 
          new SparseTableRMQ(Arrays.copyOfRange(elems, i*b, end)); 
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

  /**
   * Evaluates RMQ(i, j) over the array stored by the constructor, returning
   * the index of the minimum value in that range.
   */
  @Override
  public int rmq(int i, int j) {

    int block1 = (int)Math.floor(i / (double)b);
    int block2 = (int)Math.floor(j / (double)b);

    int minBlock = blockRMQ.rmq(block1 + 1, block2 - 1);

    int min1_idx = cartesianMin(block1, i % b, Math.min(j, (block1 + 1) * b - 1) % b);
    int min2_idx = cartesianMin(block2, Math.max(i,block2 * b) % b, j % b);
    int min3_idx = cartesianMin(minBlock, 0, b-1);

    int min_idx = min1_idx;
    if (min_idx == -1 || min2_idx != -1 && elems[min2_idx] < elems[min_idx]) 
      min_idx = min2_idx;
    if (min_idx == -1 || min3_idx != -1 && elems[min3_idx] < elems[min_idx]) 
      min_idx = min3_idx;

    return min_idx;        
  }
  // i and j are indexes within the particular block
  private int cartesianMin(int block, int i, int j){
    if (j < i || block < 0) return -1;
    SparseTableRMQ block_rmq = blockRMQArray[cartesianCodes[block]];
    return block * b + block_rmq.rmq(i, j);
  }
}
