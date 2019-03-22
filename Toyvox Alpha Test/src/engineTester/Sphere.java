package engineTester;

import space3d.*;
import java.awt.Color;
import space3d.objects.TransformableObject;

public class Sphere extends TransformableObject {

    public double getRadius() {
        return radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    public Color getColor() {
        return color;
    }

    public void setColor(Color color) {
        this.color = color;
    }

    //The radius of the sphere. Duh.
    private double radius;

    //The color of the sphere
    private Color color;

    public Sphere(double r, Vector position) {
        radius = r;
        setPosition(position);
    }
}
