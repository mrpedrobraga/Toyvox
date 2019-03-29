package voxel;

import java.awt.Color;

public class VoxelModel {

    Color[][][] model;

    public VoxelModel(Color[][][] voxelModel) {

        this.model = voxelModel;
    }

    public void setVoxelAt(int x, int y, int z, Color color) {
        model[z][y][x] = color;
    }

    public Color getVoxelAt(int x, int y, int z) {
        return model[z][y][x];
    }

    public int widthX() {
        return model[0][0].length;
    }

    public int widthY() {
        return model[0].length;
    }

    public int widthZ() {
        return model.length;
    }
}
