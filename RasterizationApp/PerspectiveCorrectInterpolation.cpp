#include "PerspectiveCorrectInterpolation.h"

int32_t PerspectiveCorrectInterpolation::MakeImage()
{
    PeCoIn::Vec3 v0 = { 13.0f,  34.0f, 114.0f };
    PeCoIn::Vec3 v1 = { 29.0f, -15.0f,  44.0f };
    PeCoIn::Vec3 v2 = { -48.0f, -10.0f,  82.0f };
    PeCoIn::Vec3 c0 = { 0, 0, 1 };
    PeCoIn::Vec3 c1 = { 0, 1, 0 };
    PeCoIn::Vec3 c2 = { 1, 0, 0 };

    const uint32_t w = 512;
    const uint32_t h = 512;

    // project triangle onto the screen
    v0[0] /= v0[2], v0[1] /= v0[2];
    v1[0] /= v1[2], v1[1] /= v1[2];
    v2[0] /= v2[2], v2[1] /= v2[2];

    // convert from screen space to NDC then raster
    v0[0] = (1 + v0[0]) * 0.5f * w, v0[1] = (1 + v0[1]) * 0.5f * h;
    v1[0] = (1 + v1[0]) * 0.5f * w, v1[1] = (1 + v1[1]) * 0.5f * h;
    v2[0] = (1 + v2[0]) * 0.5f * w, v2[1] = (1 + v2[1]) * 0.5f * h;

#ifdef PERSP_CORRECT
    // divide vetex-attribute by the vertex z-coordinate
    c0[0] /= v0[2], c0[1] /= v0[2], c0[2] /= v0[2];
    c1[0] /= v1[2], c1[1] /= v1[2], c1[2] /= v1[2];
    c2[0] /= v2[2], c2[1] /= v2[2], c2[2] /= v2[2];
    // pre-compute 1 over z
    v0[2] = 1 / v0[2], v1[2] = 1 / v1[2], v2[2] = 1 / v2[2];
#endif

    PeCoIn::Rgb* framebuffer = new PeCoIn::Rgb[w * h];
    memset(framebuffer, 0x0, w * h * 3);

    float area = edgeFunction(v0, v1, v2);

    for (uint32_t j = 0; j < h; ++j)
    {
        for (uint32_t i = 0; i < w; ++i)
        {
            PeCoIn::Vec3 p = { i + 0.5f, h - j + 0.5f, 0.f };
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
#ifdef PERSP_CORRECT
                float z = 1 / (w0 * v0[2] + w1 * v1[2] + w2 * v2[2]);
                // if we use perspective correct interpolation we need to
                // multiply the result of this interpolation by z, the depth
                // of the point on the 3D triangle that the pixel overlaps.
                r *= z, g *= z, b *= z;
#endif
                framebuffer[j * w + i][0] = (uint8_t)(r * 255);
                framebuffer[j * w + i][1] = (uint8_t)(g * 255);
                framebuffer[j * w + i][2] = (uint8_t)(b * 255);
            }
        }
    }

    std::ofstream ofs;
    ofs.open("./raster2d.ppm");
    ofs << "P6\n" << w << " " << h << "\n255\n";
    ofs.write((char*)framebuffer, w * h * 3);
    ofs.close();

    delete[] framebuffer;
    return 0;
}