#include "heads.h"

/* * * * *
 * 在屏幕上显示jpeg格式图片，可调整大小、位置
 * 参数：
 *  fp: 文件描述符
 *  ffector: 缩放参数、影响图片实际显示大小。
 *  xmove: 图片在屏幕水平移动的距离
 *  ymove: 图片在屏幕垂直移动的距离
 * * * * */
void showjpeg(FILE *fp, double ffector, int xmove, int ymove)
{
    struct jpeg_decompress_struct dinfo;
    struct jpeg_error_mgr jerr;
    dinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&dinfo);
    jpeg_stdio_src(&dinfo, fp);
    jpeg_read_header(&dinfo, TRUE);
    jpeg_start_decompress(&dinfo);

    int x = dinfo.output_width;
    int y = dinfo.output_height;
    unsigned char *buf = malloc(x * dinfo.output_components);
    unsigned char *p = buf;

    //fprintf(stderr, "Line %d x = %d , y = %d , depth = %d\n", __LINE__, x, y, dinfo.output_components);

    int w, h;
    int color;
    unsigned char a, r, g, b;
    double fectorx;
    double fectory;
    if (ffector > 32)
        ffector = 32;
    //fprintf(stderr, "jpg : Line %d ffector = %lf\n", __LINE__, ffector);
    if (x >= y)
    {
        fectorx = abs(x) / X * ffector;
        fectory = abs(y) / Y * ffector;
    }
    else
    {
        fectorx = abs(x) / Y * ffector;
        fectory = abs(y) / X * ffector;
    }

    int offset;
    p = buf;
    fprintf(stderr, "jpg : Line %d ymove = %d, ans = %d\n", __LINE__, ymove);
    if (x >= y)
    {
        for (h = 0; h < 480; h++)
        {
            while ((int)((h + ymove) * fectory) >= 0 && dinfo.output_scanline < (int)((h + ymove) * fectory) && dinfo.output_scanline < y)
            {
                jpeg_read_scanlines(&dinfo, &buf, 1);
            }

            for (w = 0; w < 800; w++)
            {
                offset = (int)((w + xmove) * fectorx) * dinfo.output_components;

                if ((offset < abs(x) * dinfo.output_components) && (int)((h + ymove) * fectory) < y && offset >= 0 && (int)((h + ymove) * fectory) >= 0)
                {
                    p = buf + offset;
                    r = *p;
                    g = *(p + 1);
                    b = *(p + 2);
                    a = (dinfo.output_components == 3) ? 0xFF : *(p + 3);
                    color = b | g << 8 | r << 16 | a << 24;
                }
                else
                    color = 0x961208;
                //           fprintf(stderr, "Line %d x = %d y = %d color = %X\n", __LINE__, w, h, color);
                //drawpoint((x>0)?w:(799-w),(y<0)?h:(479-h),color);
                drawpoint((x > 0) ? w : (799 - w), (y < 0) ? (479 - h) : h, color);
            }
        }
    }
    else
    {
        for (h = 0; h < 800; h++)
        {
            while ((int)((h + xmove) * fectory) >= 0 && dinfo.output_scanline < (int)((h + xmove) * fectory) && dinfo.output_scanline < y)
            {
                jpeg_read_scanlines(&dinfo, &buf, 1);
            }

            for (w = 0; w < 480; w++)
            {
                offset = (int)((w + ymove) * fectorx) * dinfo.output_components;

                if ((offset < abs(x) * dinfo.output_components) && (int)((h + xmove) * fectory) < y && offset >= 0 && (int)((h + xmove) * fectory) >= 0)
                {
                    p = buf + offset;
                    r = *p;
                    g = *(p + 1);
                    b = *(p + 2);
                    a = (dinfo.output_components == 3) ? 0xFF : *(p + 3);
                    color = b | g << 8 | r << 16 | a << 24;
                }
                else
                    color = 0x961208;
                //           fprintf(stderr, "Line %d x = %d y = %d color = %X\n", __LINE__, w, h, color);
                //drawpoint((x>0)?w:(799-w),(y<0)?h:(479-h),color);
                drawpoint((y < 0) ? (799 - h) : h, (x < 0) ? w : (479 - w), color);
            }
        }
    }
    while (dinfo.output_scanline < y)
    {
        jpeg_read_scanlines(&dinfo, &buf, 1);
    }
    //    fprintf(stderr, "Line %d\n", __LINE__);
    jpeg_finish_decompress(&dinfo);
    jpeg_destroy_decompress(&dinfo);
    free(buf);
}
