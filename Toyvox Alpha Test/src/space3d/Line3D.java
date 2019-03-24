package space3d;

import static space3d.VMathUtils.*;

public class Line3D {

    Vector tail;
    Vector tip;

    /**
     * Creates the line as a ray, shot from the origin at an angle
     * @param origin the xyz the ray was shot from.
     * @param length the length of the ray
     * @param hRot the horizontal rotation of the ray from the X axis
     * @param vRot the vertical rotation of the ray from the X axis
     */
    public Line3D(Vector origin, double length, double hRot, double vRot) {

        tail = origin;
        Vector tip = new Vector(0,
                                0,
                                0);
    }

    /**
     * Creates the line as the diagonal of a rectangular prism
     * @param origin the origin of the shape
     * @param size the size of the shape
     */
    public Line3D(Vector origin, Vector size) {

        tail = origin;
        tip = add(origin, size);
    }

    /**
     * An empty constructor, so you can manually set each Vector
     */
    public Line3D() {
        tail = new Vector(0,0,0);
        tip = new Vector(1,0,0);
    }
}
