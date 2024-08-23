using namespace std;

namespace TTFONT
{
    // ����TrueType�����ļ�ͷ�Ľṹ
    struct TTFHeader {
        uint32_t version;
        uint16_t numTables;
        uint16_t searchRange;
        uint16_t entrySelector;
        uint16_t rangeShift;
    };

    // ����TrueType�����Ľṹ
    struct TTFTable {
        char tag[4];
        uint32_t checksum;
        uint32_t offset;
        uint32_t length;
    };

    // ��������������Ľṹ
    struct GlyphPoint {
        float x, y;
        bool onCurve;
    };

    // �������εĽṹ
    struct Glyph {
        uint16_t numContours;
        float xMin, yMin, xMax, yMax;
        vector<GlyphPoint> points;
    };

    // ����ȫ�ֱ���
    Glyph glyph;
    float scale = 1.0f;

    // ��ȡ��������
    void readGlyphData(ifstream& file) {
        // ��ȡ�����ε�����
        if (glyph.numContours > 0) {
            // ��ȡÿ�������ĵ���
            vector<uint16_t> endPtsOfContours(glyph.numContours);
            for (int i = 0; i < glyph.numContours; ++i) {
                file.read(reinterpret_cast<char*>(&endPtsOfContours[i]), sizeof(uint16_t));
            }

            // ��ȡ�����Сֵ
            file.read(reinterpret_cast<char*>(&glyph.xMin), sizeof(float));
            file.read(reinterpret_cast<char*>(&glyph.yMin), sizeof(float));
            file.read(reinterpret_cast<char*>(&glyph.xMax), sizeof(float));
            file.read(reinterpret_cast<char*>(&glyph.yMax), sizeof(float));

            // ��ȡÿ����������״̬
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
        // ��ȡ�������ε�����
        else {
            // TODO: ʵ�ָ������εĶ�ȡ
        }
    }

    // ��ȡ����
    void readGlyph(ifstream& file, uint16_t glyphIndex) {
        // ��ȡ���α��ƫ�����ͳ���
        TTFTable glyphTable;
        for (int i = 0; i < 16; ++i) {
            file.read(reinterpret_cast<char*>(&glyphTable), sizeof(TTFTable));
            if (strncmp(glyphTable.tag, "glyf", 4) == 0) {
                break;
            }
        }

        // �����ļ�ָ�뵽�������ݵ���ʼλ��
        file.seekg(glyphTable.offset + glyphIndex * sizeof(uint16_t), ios::beg);
        uint16_t glyphOffset;
        file.read(reinterpret_cast<char*>(&glyphOffset), sizeof(uint16_t));
        file.seekg(glyphTable.offset + glyphOffset, ios::beg);

        // ��ȡ�����λ򸴺����ε�����
        file.read(reinterpret_cast<char*>(&glyph.numContours), sizeof(int16_t));
        readGlyphData(file);
    }

    // ��ȡ������Ϣ
    void readGlyphInfo(ifstream& file, uint16_t glyphIndex) {
        // ��ȡ���α��ƫ�����ͳ���
        TTFTable locaTable;
        for (int i = 0; i < 16; ++i) {
            file.read(reinterpret_cast<char*>(&locaTable), sizeof(TTFTable));
            if (strncmp(locaTable.tag, "loca", 4) == 0) {
                break;
            }
        }

        // ��ȡ�������ݵ�ƫ�����ͳ���
        uint32_t offset, length;
        file.seekg(locaTable.offset + glyphIndex * sizeof(uint32_t), ios::beg);
        file.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(&length), sizeof(uint32_t));

        // �����ļ�ָ�뵽�������ݵ���ʼλ��
        file.seekg(offset, ios::beg);

        // ��ȡ����
        readGlyph(file, glyphIndex);
    }

    // ��ȡ�����ļ�
    void readFontFile(crstr filename) {
        // �������ļ�
        ifstream file(filename.c_str(), ios::binary);

        // ��ȡ�����ļ�ͷ
        TTFHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(TTFHeader));

        // ��ȡ������Ϣ
        readGlyphInfo(file, 0);

        // �ر��ļ�
        file.close();
    }

    // ��������
    void drawGlyph() {
        // ������ɫ
        glColor3f(1.0f, 1.0f, 1.0f);

        // ��ʼ����
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < glyph.points.size(); ++i) {
            if (glyph.points[i].onCurve) {
                glVertex2f(glyph.points[i].x * scale, glyph.points[i].y * scale);
            }
            else {
                // TODO: ʵ�ֶ��κ����α��������ߵĻ���
            }
        }
        glEnd();
    }
}