using namespace std;

namespace TTFONT
{
    // 定义TrueType字体文件头的结构
    struct TTFHeader {
        uint32_t version;
        uint16_t numTables;
        uint16_t searchRange;
        uint16_t entrySelector;
        uint16_t rangeShift;
    };

    // 定义TrueType字体表的结构
    struct TTFTable {
        char tag[4];
        uint32_t checksum;
        uint32_t offset;
        uint32_t length;
    };

    // 定义字形轮廓点的结构
    struct GlyphPoint {
        float x, y;
        bool onCurve;
    };

    // 定义字形的结构
    struct Glyph {
        uint16_t numContours;
        float xMin, yMin, xMax, yMax;
        vector<GlyphPoint> points;
    };

    // 定义全局变量
    Glyph glyph;
    float scale = 1.0f;

    // 读取字形数据
    void readGlyphData(ifstream& file) {
        // 读取简单字形的数据
        if (glyph.numContours > 0) {
            // 读取每个轮廓的点数
            vector<uint16_t> endPtsOfContours(glyph.numContours);
            for (int i = 0; i < glyph.numContours; ++i) {
                file.read(reinterpret_cast<char*>(&endPtsOfContours[i]), sizeof(uint16_t));
            }

            // 读取最大最小值
            file.read(reinterpret_cast<char*>(&glyph.xMin), sizeof(float));
            file.read(reinterpret_cast<char*>(&glyph.yMin), sizeof(float));
            file.read(reinterpret_cast<char*>(&glyph.xMax), sizeof(float));
            file.read(reinterpret_cast<char*>(&glyph.yMax), sizeof(float));

            // 读取每个点的坐标和状态
            for (int i = 0; i < endPtsOfContours.back() + 1; ++i) {
                GlyphPoint point;
                file.read(reinterpret_cast<char*>(&point.x), sizeof(int16_t));
                file.read(reinterpret_cast<char*>(&point.y), sizeof(int16_t));
                if (i < endPtsOfContours[0] || i == 0) {
                    point.onCurve = true;
                }
                else {
                    point.onCurve = false;
                }
                glyph.points.push_back(point);
            }
        }
        // 读取复合字形的数据
        else {
            // TODO: 实现复合字形的读取
        }
    }

    // 读取字形
    void readGlyph(ifstream& file, uint16_t glyphIndex) {
        // 获取字形表的偏移量和长度
        TTFTable glyphTable;
        for (int i = 0; i < 16; ++i) {
            file.read(reinterpret_cast<char*>(&glyphTable), sizeof(TTFTable));
            if (strncmp(glyphTable.tag, "glyf", 4) == 0) {
                break;
            }
        }

        // 设置文件指针到字形数据的起始位置
        file.seekg(glyphTable.offset + glyphIndex * sizeof(uint16_t), ios::beg);
        uint16_t glyphOffset;
        file.read(reinterpret_cast<char*>(&glyphOffset), sizeof(uint16_t));
        file.seekg(glyphTable.offset + glyphOffset, ios::beg);

        // 读取简单字形或复合字形的数据
        file.read(reinterpret_cast<char*>(&glyph.numContours), sizeof(int16_t));
        readGlyphData(file);
    }

    // 读取字形信息
    void readGlyphInfo(ifstream& file, uint16_t glyphIndex) {
        // 获取字形表的偏移量和长度
        TTFTable locaTable;
        for (int i = 0; i < 16; ++i) {
            file.read(reinterpret_cast<char*>(&locaTable), sizeof(TTFTable));
            if (strncmp(locaTable.tag, "loca", 4) == 0) {
                break;
            }
        }

        // 获取字形数据的偏移量和长度
        uint32_t offset, length;
        file.seekg(locaTable.offset + glyphIndex * sizeof(uint32_t), ios::beg);
        file.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(&length), sizeof(uint32_t));

        // 设置文件指针到字形数据的起始位置
        file.seekg(offset, ios::beg);

        // 读取字形
        readGlyph(file, glyphIndex);
    }

    // 读取字体文件
    void readFontFile(crstr filename) {
        // 打开字体文件
        ifstream file(filename.c_str(), ios::binary);

        // 读取字体文件头
        TTFHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(TTFHeader));

        // 读取字形信息
        readGlyphInfo(file, 0);

        // 关闭文件
        file.close();
    }

    // 绘制字形
    void drawGlyph() {
        // 设置颜色
        glColor3f(1.0f, 1.0f, 1.0f);

        // 开始绘制
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < glyph.points.size(); ++i) {
            if (glyph.points[i].onCurve) {
                glVertex2f(glyph.points[i].x * scale, glyph.points[i].y * scale);
            }
            else {
                // TODO: 实现二次和三次贝塞尔曲线的绘制
            }
        }
        glEnd();
    }
}