#pragma once
//[header]
// A practical implementation of the rasterization algorithm.
//[/header]
//[compile]
// Download the raster3d.cpp, cow.h and geometry.h files to the same folder.
// Open a shell/terminal, and run the following command where the files are saved:
//
// c++ -o raster3d raster3d.cpp  -std=c++11 -O3
//
// Run with: ./raster3d. Open the file ./output.png in Photoshop or any program
// reading PPM files.
//[/compile]
//[ignore]
// Copyright (C) 2012  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//[/ignore]

#include "Geometry.h"
#include <fstream>
#include <chrono>

class Raster3D
{
    enum FitResolutionGate;
public:
    static int32_t Render();

private:
//[comment]
// Compute screen coordinates based on a physically-based camera model
// http://www.scratchapixel.com/lessons/3d-basic-rendering/3d-viewing-pinhole-camera
//[/comment]
    static void computeScreenCoordinates(
        const float& filmApertureWidth,
        const float& filmApertureHeight,
        const uint32_t& imageWidth,
        const uint32_t& imageHeight,
        const FitResolutionGate& fitFilm,
        const float& nearClippingPLane,
        const float& focalLength,
        float& top, float& bottom, float& left, float& right
    );

    //[comment]
    // Compute vertex raster screen coordinates.
    // Vertices are defined in world space. They are then converted to camera space,
    // then to NDC space (in the range [-1,1]) and then to raster space.
    // The z-coordinates of the vertex in raster space is set with the z-coordinate
    // of the vertex in camera space.
    //[/comment]
    static void convertToRaster(
        const Vec3f& vertexWorld,
        const Matrix44f& worldToCamera,
        const float& l,
        const float& r,
        const float& t,
        const float& b,
        const float& near,
        const uint32_t& imageWidth,
        const uint32_t& imageHeight,
        Vec3f& vertexRaster
    );

    static float min3(const float& a, const float& b, const float& c)
    {
        return std::min(a, std::min(b, c));
    }

    static float max3(const float& a, const float& b, const float& c)
    {
        return std::max(a, std::max(b, c));
    }

    static float edgeFunction(const Vec3f& a, const Vec3f& b, const Vec3f& c)
    {
        return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
    }

private:
    static const uint32_t imageWidth = 640;
    static const uint32_t imageHeight = 480;
    static const Matrix44f worldToCamera;

    static const uint32_t ntris = 3156;
    static const float nearClippingPLane;
    static const float farClippingPLane;
    static float focalLength; // in mm
    // 35mm Full Aperture in inches
    static float filmApertureWidth;
    static float filmApertureHeight;
    static const float inchToMm;
    enum FitResolutionGate { kFill = 0, kOverscan };
};
