package space3d;

public final class VMathUtils {

    /**
     *
     * @param x1 the first point X
     * @param y1 the first point Y
     * @param z1 the first point Z
     * @param x2 the second point X
     * @param y2 the second point Y
     * @param z2 the second point Z
     * @return the distance between the points (x1, y1, z1) and (x2, y2, z2)
     */
    public static double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
        return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2) + Math.pow(z2 - z1, 2));
    }

    public static double sin(double x) {
        return Math.sin(Math.toRadians(x));
    }

    public static double cos(double x) {
        return Math.cos(Math.toRadians(x));
    }

    public static double tan(double x) {
        return Math.tan(Math.toRadians(x));
    }

    public static double asin(double x) {
        return Math.toDegrees(Math.asin(x));
    }

    public static double acos(double x) {
        return Math.toDegrees(Math.acos(x));
    }

    public static double atan(double x) {
        return Math.toDegrees(Math.atan(x));
    }

    /**
     *
     * @param x the x of the world
     * @param y the y of the world
     * @param z the z of the world
     * @return the Isometric coordinate X, on screen
     */
    public static int isoGetX(double x, double y, double z) {
        return (int) Math.round(x * cos(45) + y * sin(45));
    }

    /**
     *
     * @param x the x of the world
     * @param y the y of the world
     * @param z the z of the world
     * @return the Isometric coordinate Y, on screen
     */
    public static int isoGetY(double x, double y, double z) {
        return (int) Math.round((-x * sin(45) + y * cos(45)) * 0.5 - z);
    }

    /**
     *
     * @param a the min value
     * @param b the max value
     * @param p the percentage between min and max
     * @return the linear interpolation from a to b at p
     */
    public static double lerp(double a, double b, double p) {

        return a + p * (b - a);
    }

    public static double getP(double min, double mid, double max) {

        return (mid - min) / (max - min);
    }

    /**
     * @param a the first angle
     * @param b the second angle
     * @return the shortest Angle distance, from a to b
     */
    public static double shortAngleDist(double a, double b) {
        double max = 360;
        double da = (a - b) % max;
        return 2 * da % max - da;
    }

    /**
     * Returns the number that is "p" between a and b.
     *
     * @param a The min value
     * @param b The max value
     * @param p The percentage
     * @return The value at the percentage
     */
    public static double angleLerp(double a, double b, double p) {
        return a + shortAngleDist(a, b) * p;
    }

    /**
     * Adds two vectors together: r = a + b
     *
     * @param a The first Vector
     * @param b The second Vector
     * @return the shortest path from a's tail to b's tip
     */
    public static Vector add(Vector... vectors) {

        double sx = 0, sy = 0, sz = 0;

        for (int i = 0; i < vectors.length; i++) {

            sx += vectors[i].getX();
            sy += vectors[i].getY();
            sz += vectors[i].getZ();
        }

        return new Vector(sx, sy, sz);
    }

    /**
     * Subtracts a vector from another: r = a - b
     *
     * @param a The vector that will be subtracted from.
     * @param b The vector that is negated.
     * @return The resulting Vector
     */
    public static Vector subtract(Vector a, Vector b) {

        return new Vector(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
    }

    /**
     * Resizes a vector, changing only it's length
     *
     * @param a the Vector to be scaled
     * @param scale the new length of the Vector, in comparison to the old one
     * @return the scaled Vector
     */
    public static Vector multiply(Vector a, double scale) {

        return new Vector(a.getX() * scale, a.getY() * scale, a.getZ() * scale);
    }

    /**
     * Rotates a vector, keeping it's lenght unchanged.
     *
     * @param a the Vector to be rotated
     * @param hROT the Horizontal rotation. Positive means clockwise
     * @param vROT the Vertical rotation. Positive means clockwise
     * @return the rotated Vector
     */
    public static Vector rotate(Vector a, double hROT, double vROT) {

        //@TODO Everything
        double l;
        double hRot;
        double vRot;

        return null;
    }

    /**
     *
     * @param a the vector where b will be projected at
     * @param b the projected vector
     * @return the length of vector b, when projected into a
     */
    public static double dotProduct(Vector a, Vector b) {

        return a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ();
    }

    public static double linePointDistance(Vector line, Vector point) {

        return 0;
    }

    public static double angleBind(double angle) {

        double r = angle;

        while (r < 0) {
            r += 360;
        }

        while (r > 360) {
            r -= 360;
        }
        
        return r;
    }
    
    public static double bind(double min, double x, double max) {
        
        return Math.min(max, Math.max(x, min));
    }
    
    public static Vector getRectVector(double length, double hRot, double vRot) {
        
        return new Vector(length * Math.cos(Math.toRadians(hRot)) * Math.cos(Math.toRadians(vRot)),
                          length * Math.cos(Math.toRadians(hRot)) * Math.sin(Math.toRadians(vRot)),
                          length * Math.sin(Math.toRadians(hRot)));
    }
    
    //TODO getPolarVector()
    
    public static double distance(Vector a, Vector b) {
        
        return Math.sqrt(Math.pow(Math.abs(b.getX() - a.getX()),2) + Math.pow(Math.abs(b.getY() - a.getY()),2) + Math.pow(Math.abs(b.getZ() - a.getZ()),2));
    }
    
    public static Vector normalize(Vector v) {
        
        return multiply(v, 1/(Math.sqrt(Math.pow(v.getX(),2) + Math.pow(v.getY(),2) + Math.pow(v.getZ(),2))));
    }
}
