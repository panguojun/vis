/**
                FeatureVector������������
                ���ڱ�ʾ������������
                ÿ���������������������ֵ��ÿ������ֵ��ʾ������ĳ�������ϵ�ȡֵ��
                �ڷ��ࡢ����������У�����ͨ���Ƚϲ�ͬ�������������������ж�����
                �Ƿ�����ͬһ������ͬһ�ء�
*/
namespace feature
{
    struct fvector
    {
        static const fvector ZERO;
        vecn v;             // �洢��������ֵ������
        string label;       // ���������ı�ǩ

        // ���캯��
        fvector() {}
        fvector(const vecn& _v, crstr l) {
            v = _v;
            label = l;
        }
    };
    const fvector fvector::ZERO = fvector();

    std::vector<std::pair<real, fvector> >    fv_list;
    void fromGTable()
    {
        for (auto& v : GTable::floatmap)
        {
            vector<string> par;
            STR::split(v.first, par, " ");
            fvector fv;
            fv.label = par.front();
            fv.v[0] = v.second;
            fv_list.emplace_back(atof(par.back().c_str()), fv);
        }
    }
    // ----------------------------
    // ��ֵ
    // ----------------------------
    fvector blend(const fvector& fv1, const fvector& fv2, real t)
    {
        if (fv1.label == fv2.label)
        {
            fvector fv;
            fv.v = blend(fv1.v, fv2.v, t);
            return fv;
        }
        return fvector::ZERO;
    }
    std::pair<fvector, fvector> getpair(const string& label, real t)
    {
        std::pair<fvector, fvector> ret;
        for (auto& it : fv_list)
        {
            if (label == it.second.label) {
                if (it.first < t)
                    ret.first = it.second;
            }
            if (label == it.second.label) {
                if (it.first > t)
                    ret.second = it.second;
            }
        }
        return ret;
    }
    real blend_alpha = 0.1;
    fvector blend(const string& label, real t)
    {
        std::pair<fvector, fvector> par = getpair(label, t);
        if (par.first.label == label)
        {
            fvector fv;
            fv.v = blend(par.first.v, par.second.v, blend_alpha);
            return fv;
        }
        return fvector::ZERO;
    }
}