import mylib
import numpy as np
if __name__ == '__main__':
    M = np.zeros(3,3)
    M = mylib.add_one(M)
    print(mylib.multyply_by_scalar(M, 2.0))
    
