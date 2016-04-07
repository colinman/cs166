package rmq;

import java.util.HashMap;

/**
 * An &lt;O(n log n), O(1)&gt; implementation of RMQ that uses a sparse table
 * to do lookups efficiently.
 *
 * You will implement this class for problem 3.ii of Problem Set One.
 */
public class SparseTableRMQ implements RMQ {

    /**
     * Maps number n to highest k such that 2^k <= n
     */
    HashMap<Integer, Integer> powerTable;
    int[][] sparseTable;
    float[] elems;

    /**
     * Creates a new SparseTableRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public SparseTableRMQ(float[] elems) {
        this.elems = elems;
        int n = elems.length;
        if (n == 0) return;

        powerTable = generatePowerTable(n);
        sparseTable = new int[n][];

        // construct propertly sized array
        for (int i = 0; i < n; i++) {
            int upper = floorLog2(n - i) + 1;
            sparseTable[i] = new int[upper];
        }

        // loop through exponents and dp
        for (int i = 0; i < floorLog2(n) + 1; i++) {
            int additional = (int)Math.pow(2, i-1);
            for (int j = 0; j < n; j++) {
                if (j + (int)Math.pow(2, i) > n) continue;
                if (i == 0) {
                    sparseTable[j][0] = j;
                    continue;
                }
                int firstMinIdx = sparseTable[j][i-1];
                int secondMinIdx = sparseTable[j + additional][i-1];
                sparseTable[j][i] = (elems[firstMinIdx] < elems[secondMinIdx]) ? firstMinIdx : secondMinIdx;
            }
        }
    }

    private HashMap<Integer, Integer> generatePowerTable(int n) {
        HashMap<Integer, Integer> table = new HashMap<Integer, Integer>();
        int exp = 0, val = 2;
        for (int i = 1; i <= n; i++) {
          if (i >= val) val = (int)Math.pow(2, ++exp + 1);
          table.put(i, exp);
        }
        return table;
    }

    private int ceilLog2(float num) {
        return (int)Math.ceil(log2(num));
    }

    private int floorLog2(float num) {
        return (int)Math.floor(log2(num));
    }

    private double log2 (float num) {
        return Math.log(num) / Math.log(2);
    }

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {
        if (elems.length == 0 || j < i) return -1;
        int k = powerTable.get(j - i + 1);
        int range1minIdx = sparseTable[i][k];
        int range2minIdx = sparseTable[j-(int)Math.pow(2,k)+1][k];
        return (elems[range1minIdx] < elems[range2minIdx]) ? range1minIdx : range2minIdx;
    }
}
