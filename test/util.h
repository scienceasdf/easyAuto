template <class T>
bool fuzzyEqual(T a, T b, T relativeErr)
{
    if((a - b * (static_cast<T>(1.0) + relativeErr)) *
       (a - b * (static_cast<T>(1.0) - relativeErr)) < 0){
        return true;
    }
    else{
        return false;
    }
}
