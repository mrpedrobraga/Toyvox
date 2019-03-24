package space3d;

import java.util.ArrayList;

public class Layout3D {

    long UIDs = 1;
    private ArrayList<Object3D> objects = new ArrayList<>();

    public void addObject(Object3D object) {

        objects.add(object);
        object.setUID(UIDs);
        UIDs++;
    }
    
    public Object3D getObject(int index) {

        return objects.get(index);
    }
}
