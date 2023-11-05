#include "NN.h"


NN::NN(size_t* i_architecture, size_t i_architecture_count)
{
	m_count = i_architecture_count - 1;
	m_biases = new myMatrix[m_count];
	m_weights = new myMatrix[m_count];
	m_activations = new myMatrix[m_count+1];//nie m_count+1?
	m_activations[0] = myMatrix(1, i_architecture[0]);


	for (size_t i = 1; i < i_architecture_count; ++i)
	{
		m_weights[i-1] = myMatrix(m_activations[i - 1].getColsNo(), i_architecture[i]);
		m_biases[i-1] = myMatrix(1, i_architecture[i]);
		m_activations[i] = myMatrix(1, i_architecture[i]);
	}
}

void NN::Forward()//i think it has to be a function
{
	//std::cout << "pre forward: \t" << this->m_activations[m_count];
	for (size_t i = 0; i < m_count; ++i)
	{
		m_activations[i + 1] = m_activations[i] * m_weights[i];
		m_activations[i + 1] = m_activations[i + 1] + m_biases[i];
		m_activations[i + 1].Activate();
	}
	//std::cout << "psot forward: \t" << this->m_activations[m_count];
}

void NN::FillRandom()
{
	for (size_t i = 0; i < m_count; ++i)
	{
		this->m_weights[i].FillRandom();
		this->m_biases[i].FillRandom();
	}

}

void NN::FillWith(float i_val)
{
	for (size_t i = 0; i < m_count; ++i)
	{
		this->m_weights[i].FillWith(i_val);
		this->m_biases[i].FillWith(i_val);
		this->m_activations[i].FillWith(i_val);

	}
		this->m_activations[this->m_count].FillWith(i_val);
}



void NN::Learn(NN g, float rate)
{
	float saved = 0.0f;
	for (size_t i = 0; i < this->m_count; ++i)
	{
		for (size_t j = 0; j < this->m_weights[i].getRowsNo(); ++j) {

			for (size_t k = 0; k < this->m_weights[i].getColsNo(); ++k) {
				
				this->m_weights[i][j][k] -= rate * g.m_weights[i][j][k];

			}

		}


		for (size_t j = 0; j < this->m_biases[i].getRowsNo(); ++j) {

			for (size_t k = 0; k < this->m_biases[i].getColsNo(); ++k) {
			
				this->m_biases[i][j][k] -= rate * g.m_biases[i][j][k];



			}

		}
	}

}

void NN::Render(Olivec_Canvas img)
{
	uint32_t background_color = 0XFFFFFFFF;
	uint32_t low_color = 0x00ff00ff;
	uint32_t high_color = 0x0000FF00;
	olivec_fill(img, background_color);
	int neuron_radius = 8;
	int layer_border_vpad = 1;//px
	int layer_border_hpad = 1;//px
	int nn_width = img.width - 2 * layer_border_hpad;
	int nn_height = img.height - 2 * layer_border_vpad;
	size_t arch_count = this->m_count + 1;
	int layer_hpad = nn_width / arch_count;
	int nn_x = img.width / 2 - nn_width / 2;
	int nn_y = img.height / 2 - nn_height / 2;

	for (size_t l = 0; l < arch_count; ++l)
	{

		int layer_vpad1 = nn_height / this->m_activations[l].getColsNo();

		for (size_t i = 0; i < this->m_activations[l].getColsNo(); ++i)
		{
			int cx1 = nn_x + l * layer_hpad + layer_hpad / 2;
			int cy1 = nn_y + i * layer_vpad1 + layer_vpad1 / 2;

			std::cout << cx1 << " , " << cy1 << "\n";
			if ((l + 1) < arch_count)
			{

				for (size_t j = 0; j < this->m_activations[l+1].getColsNo(); ++j)
				{
					int layer_vpad2 = nn_height / this->m_activations[l+1].getColsNo();
					int cx2 = nn_x + (l + 1) * layer_hpad + layer_hpad / 2;
					int cy2 = nn_y + j * layer_vpad2 + layer_vpad2 / 2;
					uint32_t alpha = floorf(255.0f * sigmoidf(this->m_weights[l][j][i]));
					uint32_t connectcion_color = 0xff000000 | low_color;
					olivec_blend_color(&connectcion_color, (alpha << (8 * 3)) | high_color);
					olivec_line(img, cx1, cy1, cx2, cy2, connectcion_color);
				}
			}

			if (l > 0)
			{
				uint32_t alpha = floorf(255.0f * sigmoidf(this->m_biases[l - 1][0][i]));
				uint32_t neuron_color = 0xff000000 | low_color;
				olivec_blend_color(&neuron_color, (alpha << (8 * 3)) | high_color);
				olivec_circle(img, cx1, cy1, neuron_radius, neuron_color);
			}
			else
			{
				olivec_circle(img, cx1, cy1, neuron_radius, 0xFFAAAAAA);

			}
		}
	}
}

void NN::RenderRaylib(int rx, int ry, int rw, int rh)
{

	Color low_color = { 0xff,0x00,0xff,0xff };
	Color high_color = { 0x00,0xff,0x00,0xff };

	int neuron_radius = rh*0.04f;
	int layer_border_vpad = 1;//px
	int layer_border_hpad = 1;//px
	int nn_width = rw - 2 * layer_border_hpad;
	int nn_height = rh- 2 * layer_border_vpad;
	size_t arch_count = this->m_count + 1;
	int layer_hpad = nn_width / arch_count;
	int nn_x = rx+rw/ 2 - nn_width / 2;
	int nn_y = ry+rh/ 2 - nn_height / 2;

	for (size_t l = 0; l < arch_count; ++l)
	{

		int layer_vpad1 = nn_height / this->m_activations[l].getColsNo();

		for (size_t i = 0; i < this->m_activations[l].getColsNo(); ++i)
		{
			int cx1 = nn_x + l * layer_hpad + layer_hpad / 2;
			int cy1 = nn_y + i * layer_vpad1 + layer_vpad1 / 2;

			if ((l + 1) < arch_count)
			{

				for (size_t j = 0; j < this->m_activations[l + 1].getColsNo(); ++j)
				{
					int layer_vpad2 = nn_height / this->m_activations[l + 1].getColsNo();
					int cx2 = nn_x + (l + 1) * layer_hpad + layer_hpad / 2;
					int cy2 = nn_y + j * layer_vpad2 + layer_vpad2 / 2;
					float value = sigmoidf(this->m_weights[l][j][i]);
					float thick = rh*0.004f*value;
					high_color.a = floorf(255.0f * value);
					DrawLineEx(Vector2(cx1, cy1), Vector2(cx2, cy2), thick, ColorAlphaBlend(low_color, high_color, WHITE));

				}
			}

						if (l > 0)
						{
							high_color.a = floorf(255.0f * sigmoidf(this->m_biases[l - 1][0][i]));
							DrawCircle(cx1, cy1, neuron_radius, ColorAlphaBlend(low_color, high_color, WHITE));
						}
						else
						{

							DrawCircle(cx1, cy1, neuron_radius, GRAY);
						}
			}
		}
	}




float NN::Cost(myMatrix In, myMatrix Out)
{
	//shouldnt i do this with temp NN?
	size_t n = In.getRowsNo();
	float c = 0.0f;
	for (size_t i = 0; i < n; ++i)
	{
		myMatrix x = myMatrix(1,In.getColsNo(),In.getRow(i));
		myMatrix y = myMatrix(1,Out.getColsNo(),Out.getRow(i));
		this->m_activations[0] = x;
		this->Forward();
		size_t q = Out.getColsNo();
		//std::cout << "y&activ:\t" << y << this->m_activations[this->m_count];
		for (size_t j = 0; j < q; ++j)
		{
			//msitake must be here somewhere.
			float d = this->m_activations[this->m_count][0][j] - y[0][j];
			c += d * d;
		}
	}
	return float(c / float(n));
}

void NN::Feed(myMatrix In)
{
	this->m_activations[0] = In;
	//std::cout << "for:\n"<<In;

	this->Forward();
	//std::cout<<"result:\t"<<this->m_activations[this->m_count];
}

std::ostream& operator<<(std::ostream& os, const NN& i_NN)
{

	for (size_t i = 0; i < i_NN.m_count; ++i)
	{
		MAT_PRINT(i_NN.m_weights[i]);
		MAT_PRINT(i_NN.m_biases[i]);
	}
	return os;
}

void finite_diff(NN& nn, NN& g, float eps, myMatrix ti, myMatrix to)//here is the problem, only the last layer is updating...
{

	float saved = 0.0f;
	float c = nn.Cost(ti, to);
	//std::cout << "pre cost:\t" << c<<"\n";
	for (size_t i = 0; i < nn.m_count; ++i)
	{
		for (size_t j = 0; j < nn.m_weights[i].getRowsNo(); ++j) {
			
			for (size_t k = 0; k < nn.m_weights[i].getColsNo(); ++k) {

				saved = nn.m_weights[i][j][k];
				//std::cout << "nn pre:" << nn;
				nn.m_weights[i][j][k]+=eps;
				//std::cout << "nn +eps: "<<eps << nn;

				float NewCost = nn.Cost(ti, to) ;
	//std::cout << "New cost: "<<i<<"\t" << NewCost << "\n";
				g.m_weights[i][j][k] =(NewCost- c) / eps;
				 nn.m_weights[i][j][k]= saved;
			
				 
			
			
			}

		}


		for (size_t j = 0; j < nn.m_biases[i].getRowsNo(); ++j) {

			for (size_t k = 0; k < nn.m_biases[i].getColsNo(); ++k) {
				saved = nn.m_biases[i][j][k];
				nn.m_biases[i][j][k] += eps;
				g.m_biases[i][j][k] = (nn.Cost(ti, to) - c) / eps;
				nn.m_biases[i][j][k] = saved;



			}

		}
	}
	
}

void back_prop(NN& nn, NN& g, myMatrix ti, myMatrix to)
{
	//i-current sample
	//l-current layer
	//j--current activation
	//k- previous activation
	size_t n = ti.getRowsNo();
	g.FillWith(0.0f);//to jewt hujowo na bank
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j <= g.m_count; ++j)
		{
			g.m_activations[j].FillWith(0.0f);
		}
		//for each sample i feed it to the neural network and forward it
		//nn.m_activations[0] = ti.getRow(i);
		//nn.Forward();
		myMatrix ToBeEaten(1, ti.getColsNo(), ti.getRow(i));
		nn.Feed(ToBeEaten);


		for (size_t j = 0; j < to.getColsNo(); ++j)
		{
			//in activations of a gradient i store the derrivatives with regard to the next (sometime imaginary) layer
			g.m_activations[g.m_count][0][j]=(nn.m_activations[nn.m_count ][0][j]-to[i][j]);	
		}
		for (size_t l = nn.m_count; l > 0; --l)
		{
			for (size_t j = 0; j < nn.m_activations[l].getColsNo(); ++j)
			{
				float a = nn.m_activations[l][0][j];
				float da = g.m_activations[l][0][j];
				g.m_biases[l - 1][0][j] += 2.0f  * da * a * (1.0f - a);
				for (size_t k = 0; k <nn.m_activations[l-1].getColsNo() ; ++k)
				{
					float pa = nn.m_activations[l - 1][0][k];
					float w = nn.m_weights[l - 1][k][j];
				
					g.m_weights[l - 1][k][j] += 2.0f * da * a*(1.0f - a)*pa;
					g.m_activations[l - 1][0][k] += 2.0f * da * a * (1.0f - a)*w;
				}
			}
		}
	}
	for (size_t i = 0; i < g.m_count; ++i)
	{
		for (size_t j = 0; j < g.m_weights[i].getRowsNo(); ++j) 
		{

			for (size_t k= 0; k < g.m_weights[i].getColsNo(); ++k)
				{	
				g.m_weights[i][j][k] /= n;
				}
		}
		for (size_t j = 0; j < g.m_biases[i].getRowsNo(); ++j) 
		{

			for (size_t k = 0; k < g.m_biases[i].getColsNo(); ++k)
			{
				g.m_biases[i][j][k] /= n;
			}
		}
	}
}
