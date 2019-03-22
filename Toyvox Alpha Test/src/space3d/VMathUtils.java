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

    public static int isoGetX(double x, double y, double z) {
        return (int) Math.round(x * cos(45) + y * sin(45));
    }

    public static int isoGetY(double x, double y, double z) {
        return (int) Math.round((-x * sin(45) + y * cos(45)) * 0.5 - z);
    }

    public static double lerp(double a, double b, double p) {
        
        return a + p * (b - a);
    }
    
    public static double getP(double min, double mid, double max){
        
        return (mid - min)/(max - min);
    }
    
    public static double shortAngleDist(double a, double b) {
        double max = Math.PI*2;
        double da = (a - b) % max;
        return 2*da % max - da;
    }

    public static double angleLerp(double a, double b, double p) {
        return a + shortAngleDist(a,b)*p;
    }
}
