package rmq;

import java.util.Arrays;

/**
 * An &lt;O(n), O(log n)&gt; implementation of the RMQ as a hybrid between
 * the sparse table (on top) and no-precomputation structure (on bottom)
 *
 * You will implement this class for problem 3.iii of Problem Set One.
 */
public class HybridRMQ implements RMQ {

  int b;
  float[] block_mins;
  float[] elems;
  SparseTableRMQ blockRMQ;


    /**
     * Creates a new HybridRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public HybridRMQ(float[] elems) {
      int n = elems.length;
      b = (int)Math.floor(Math.log(n)/Math.log(2)) + 1;
      this.elems = elems;

      int num_mins = (int)Math.ceil(n / b);
      block_mins = new float [num_mins];

      for (int i = 0; i < num_mins; i++) {
        int min_idx = -1;
        int end = (i*b + b > n) ? n : i*b + b;
        for (int j = i*b; j < end; j++) {
          if (min_idx == -1 || elems[j] < elems[min_idx]) min_idx = j;
        }
        block_mins[i] = elems[min_idx];
      }

      blockRMQ = new SparseTableRMQ (block_mins);

    }

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {

      int block1 = (int)Math.floor(i / b);
      int block2 = (int)Math.floor(j / b);
      int minBlock = blockRMQ.rmq(block1 + 1, block2 - 1);

      int min1_idx = scanForMin(i, Math.min(j, (block1 + 1) * b));
      int min2_idx = scanForMin(Math.max(i,block2 * b), j);
      int min3_idx = scanForMin(Math.max(i,minBlock * b), Math.min((minBlock + 1) * b, j));

      int min_idx = min1_idx;
      if (min_idx == -1 || min2_idx != -1 && elems[min2_idx] < elems[min_idx]) min_idx = min2_idx;
      if (min_idx == -1 || min3_idx != -1 && elems[min3_idx] < elems[min_idx]) min_idx = min3_idx;

      return min_idx;       
    }

    private int scanForMin(int i, int j) {
      if (i < 0) i = 0;
      if (j >= elems.length) j = elems.length - 1;

      int min_idx = -1;
      for (int k = i; k <= j; k++) {
        if (min_idx == -1 || elems[k] < elems[min_idx]) min_idx = k;
      }
      return min_idx;
    }

}
