package rmq;
/**
 * An &lt;O(n<sup>2</sup>), O(1)&gt; implementation of RMQ that precomputes the
 * value of RMQ_A(i, j) for all possible i and j.
 *
 * You will implement this class for problem 3.i of Problem Set One.
 */
public class PrecomputedRMQ implements RMQ {
	
	int[][] precomputedTable;

    /**
     * Creates a new PrecomputedRMQ structure to answer queries about the
     * array given by elems.
     *
     * @elems The array over which RMQ should be computed.
     */
    public PrecomputedRMQ(float[] elems) {
		int n = elems.length;
		precomputedTable = new int[n][n];

		int index_rmq = -1; // Initial value should never be used 
   		for (int i = 0; i < n; i++){
			for (int j = i; j < n; j++){
				if (i == j || elems[j] < elems[index_rmq]){
					index_rmq = j; 
				}
				precomputedTable[i][j] = index_rmq;	
			}
		}		 
	}

    /**
     * Evaluates RMQ(i, j) over the array stored by the constructor, returning
     * the index of the minimum value in that range.
     */
    @Override
    public int rmq(int i, int j) {
		return precomputedTable[i][j];
    }

}
