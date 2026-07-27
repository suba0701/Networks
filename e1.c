int findComplement(int num) 
{
    
    long i = 1;
    
    
    while (i <= num) {
        i = i * 2; 
    }
    
    
    long mask = i - 1;
    
    
    return mask - num;
}

