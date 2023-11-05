#include "Matrix.h"

float sigmoidf(float x)
{
    return 1.0f / (1.0f + expf(-x));
}
float cost(myMatrix m, myMatrix ti, myMatrix to)
{

    size_t n = ti.getRowsNo();
    for (size_t i = 0; i < n; ++i)
    {
        //Matrix x (ti.getRow(i));
        //Matrix y(to.getRow(i));

    }

    return 0;
}
std::ostream& operator<<(std::ostream& os, myMatrix& i_m)
{
    std::cout << "\t[\n";
    for (size_t i = 0; i < i_m.m_rows; ++i)
    {

    std::cout << "\t";
        for (size_t j = 0; j < i_m.m_cols; ++j)
        {
            printf("%f\t", i_m[i][j]);
            //std::cout << res << "   ";

        }
        std::cout << "\n";
    }
    std::cout << "\t]\n";
    return os;

}

myMatrix operator+(myMatrix& mat_left, myMatrix& mat_right)
{
    myMatrix buffer(mat_left.m_rows, mat_left.m_cols);
    for (size_t i = 0; i < mat_left.m_rows; ++i)
    {
        for (size_t j = 0; j < mat_left.m_cols; ++j)
        {
            //std::cout <<"right: "<< mat_right[i][j] << " left: "<< *this[i][j]<<" " << std::endl;
            buffer[i][j]=mat_left[i][j] + mat_right[i][j];

        }

    }
    return buffer;
}

myMatrix operator*(myMatrix mat_left, myMatrix& mat_right)
{
    myMatrix buffer(mat_left.getRowsNo(), mat_right.getColsNo());



    for (size_t i = 0; i < mat_left.getRowsNo(); ++i)
    {
        for (size_t j = 0; j < mat_right.getColsNo(); ++j)
        {
            float sum = 0;
            for (size_t k = 0; k < mat_left.getColsNo(); ++k)
            {
            sum+=mat_left[i][k] * mat_right[k][j];

            }
            buffer[i][j] = sum;

        }
    }
    return buffer;
}





//myMatrix& myMatrix::getRow(size_t row)
//{
//    myMatrix buffer= myMatrix( 1, m_cols, &m_data[m_cols*row]);//?? czy tutaj nie dzialam na tym samym XD?
//    return buffer;
//}
float* myMatrix::getRow(size_t row)
{
    float* rowData = new float[m_cols];
    std::memcpy(rowData, &m_data[m_cols * row], m_cols * sizeof(float));
    return rowData;
}

myMatrix& myMatrix::getCol(size_t col)
{
    myMatrix* buffer =new myMatrix(m_rows,1);
    for (size_t i = 0; i < m_rows; ++i)
    {
      
        *buffer[0][i] = *this[i][col];
        
    }
    return *buffer;
}

myMatrix& myMatrix::popRow(size_t row)
{
    float* buf;
    float* buf_data = new float[(m_rows - 1) * m_cols]; 

    buf = this->getRow(row);

    size_t buf_index = 0;
    for (size_t i = 0; i < m_rows; ++i)
    {
        if (i != row) 
        {
            for (size_t j = 0; j < m_cols; ++j)
            {
                buf_data[buf_index++] = m_data[i * m_cols + j];
            }
        }
    }
    myMatrix retMat(1, this->getColsNo(), buf);
    //delete[] m_data;
    m_data = buf_data;
    --m_rows; 
    return retMat;
}


void myMatrix::FillRandom()
{
    for (size_t i = 0; i < m_rows*m_cols; ++i)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f); 
        m_data[i] = dist(rng);

    }
}

void myMatrix::FillWith(float i_val)
{
    for (size_t i = 0; i < m_rows*m_cols; ++i)
    {
        m_data[i] = i_val;
    }
}

void myMatrix::Save(FILE* out)
{
    const char* magic = "nn.h.mat";
    fwrite(magic, strlen(magic), 1, out);
    fwrite(&m_rows, sizeof(m_rows), 1, out);
    fwrite(&m_cols, sizeof(m_cols), 1, out);
    size_t n=fwrite(m_data, sizeof(*m_data), m_rows * m_cols, out);
    while (n < m_rows * m_cols&&!ferror(out))
    {
     size_t   m = fwrite(m_data+n, sizeof(*m_data), m_rows * m_cols-n, out);
     n += m;
    }
}

void myMatrix::Load(FILE* in)
{

}

//fisher yates shuffle
void myMatrix::ShuffleRows()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    for (size_t i = 0; i < this->m_rows; ++i)
    {
        
        std::uniform_int_distribution<int> dist(0, this->m_rows-i-1);
        size_t j = i+dist(rng);
        for (size_t k = 0; k < this->m_cols; ++k)
        {

         
            if(i!=j)
            {
                float t = m_data[i*m_cols+k];
                m_data[i * m_cols + k]= m_data[j * m_cols + k];
                m_data[j * m_cols + k] = t;
            }
        }
    }

}

void myMatrix::AddRow(float* i_row)
{
    ++m_rows;
    float* buf_data = new float[m_cols * m_rows];
    for (size_t i = 0; i < m_rows - 1; ++i)
    {
        for (size_t j = 0; j < m_cols; ++j)
        {
            buf_data[i * m_cols + j] = m_data[i * m_cols + j];
        }
    }
    for (size_t j = 0; j < m_cols; ++j)
    {
        buf_data[(m_rows - 1) * m_cols + j] = i_row[j];
    }
    delete[] m_data;
    m_data = buf_data;
}

void myMatrix::Activate()
{
    for (size_t i = 0; i < m_rows; ++i)
    {
        for (size_t j = 0; j < m_cols; ++j)
        {
            m_data[i*m_rows+j] = sigmoidf(m_data[i * m_rows +j]);
        }

    }
}




myMatrix::myMatrix(size_t i_rows, size_t i_cols, float* i_data):
    m_cols(i_cols),m_rows(i_rows),m_data(i_data){}

myMatrix::myMatrix(size_t i_rows, size_t i_cols):
    m_cols(i_cols), m_rows(i_rows)
{
    m_data = new float[m_rows * m_cols];
  

    for (size_t i = 0; i < m_rows*m_cols; ++i)
    {
                m_data[i] = 0;

    }

}


myMatrix::myMatrix(myMatrix& i_mat)
{
    m_rows = i_mat.m_rows;
    m_cols = i_mat.m_cols;
    m_data = i_mat.m_data;
}

myMatrix::myMatrix()//this shouldn't be a thing unless we want to implement safety measures
{
    m_rows = 0;
    m_cols = 0;
    m_data = nullptr;
}

myMatrix::~myMatrix()//.....XDDD

{

}
