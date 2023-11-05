#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "NN.h"
#define BITS 4
uint32_t img_pixels[IMG_WIDTH * IMG_HEIGHT];



void plot_min_max(std::vector<float>plot, float* min, float* max)
{
	*min = FLT_MAX;
	*max = FLT_MIN;
	for (auto& i : plot)
	{
		if (i < *min)*min = i;
		if (i > *max)*max = i;
	}
}

void plot_cost(std::vector<float>plot, int rx, int ry, int rw, int rh)
{
	float min, max;
	plot_min_max(plot, &min, &max);
	size_t n = plot.size();
	if (n < 1000)n = 1000;
	if (min > 0.0f)min = 0.0f;
	min = 0.0f;
	for (size_t i=0;i+1<plot.size();++i)
	{
		float x1 = rx+float(rw) / plot.size() * i;
		float y1 = ry+(1-(plot[i] - min) / (max - min)) * rh;
		float x2 = rx + float(rw) / plot.size() * (i+1);
		float y2 = ry + (1-(plot[i+1] - min) / (max - min)) * rh;
		DrawLineEx(Vector2{ x1, y1 }, Vector2{ x2, y2 }, rh * 0.004f, RED);
	}

}

int main(void)
{
	std::vector<float>plot;
	size_t n = (1 << BITS);
	size_t rows = n * n;
	myMatrix t(rows, 3 * BITS + 1);
	//myMatrix ti(rows, 2 * BITS);
	//myMatrix to(rows, BITS + 1);
	for (size_t i = 0; i < rows; ++i)
	{
		size_t x = i / n;
		size_t y = i % n;
		size_t z = x + y;
		for (size_t j = 0; j < BITS; ++j)
		{
			t[i][j]= (x >> j) & 1;//0-4
			t[i][j+BITS]= (y >> j) & 1;//4-8
			t[i][j+2*BITS]= (z >> j) & 1;//8-12

			/*ti[i][j] = (x >> j) & 1;
			ti[i][j + BITS] = (y >> j) & 1;
			to[i][j] = (z >> j) & 1;*/
		}
			//to[i][BITS] = (z>=n);
			t[i][3*BITS]= (z >= n);//13
	}


	//float data [] = {1, 0, 2, 0, 0, 3, 0, 1, 0};
	// myMatrix test(3, 3, &data[0]);
	// myMatrix t1(0,2);
	// myMatrix t2(0,1);
	// for (size_t i = 0; i < 3; ++i)
	// {
	//	 myMatrix current_row = myMatrix(1,test.getColsNo(),test.getRow(i));
	//	 t1.AddRow(current_row.getData(0));
	//	 t2.AddRow(current_row.getData(t1.getColsNo()));
	// }
	//std::cout << test;

	//myMatrix popped = test.popRow(1);
	//std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~\n" << popped;
	//myMatrix gottenrow=myMatrix(1,popped.getColsNo(),popped.getRow(0));
	//std::cout << gottenrow;
	//std::cout << test;

	//return 0;

	t.ShuffleRows();
	/*std::cout << t;
	myMatrix ti(rows, 2 * BITS,t.getData(0));
	myMatrix to(rows, BITS + 1,t.getData(ti.getColsNo()));*/
	size_t arch[] = { 2*BITS,4*BITS , BITS + 1 };
	size_t arch_count = sizeof(arch) / sizeof(arch[0]);
	NN nn(arch, arch_count);
	NN g(arch, arch_count);

	nn.FillRandom();
	std::cout << nn;



	const int screenWidth = 800;
	const int screenHeight = 600;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "NN in raylib");
	SetTargetFPS(144);
	size_t epoch = 0;
	size_t max_epoch = 100*1000;

	size_t batch_size = 8;
	size_t batch_count = (t.getRowsNo()+batch_size-1) / batch_size;
	size_t batches_per_frame = 50;
	size_t batch_begin = 0;
	float cost = 0.0f;
	float rate = 0.5f;
	bool paused = true;
	Color background_color = { 0x18,0x18,0x18,0xFF };
	

	

	//for (size_t i = 0; i < batch_count; ++i)
	//{
	//	size_t batch_current = i;
	//	myMatrix batch_ti(batch_size, 2 * BITS, t.getData(batch_current * batch_size));
	//	myMatrix batch_to(batch_size, BITS + 1, t.getData(batch_current * batch_size + batch_ti.getColsNo()));
	//	std::cout << batch_ti << batch_to;
	//	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n\n\n";
	//}
	//return 0;
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_SPACE))
		{
			paused = !paused;
		}
		if (IsKeyPressed(KEY_R))
		{
			epoch = 0;
			cost = 0.0f;
			nn.FillRandom();
			plot.clear();
		}


		for (size_t i = 0; i < batches_per_frame&&!paused&& epoch < max_epoch; ++i)
		{
			size_t size = batch_size;
			//myMatrix batch_ti(size, 2 * BITS, t.getData(batch_begin));
			//myMatrix batch_to(size, BITS + 1, t.getData(batch_begin + batch_ti.getColsNo()));
			if (batch_begin + batch_size >= t.getRowsNo())
			{
				size = t.getRowsNo() - batch_begin;
			}
			myMatrix batch_ti(0,2*BITS);
			myMatrix batch_to(0,BITS+1);
			for (size_t j = 0; j < size; ++j)
			{
				myMatrix current_row = myMatrix(1,t.getColsNo(),t.getRow(batch_begin+j));

				batch_ti.AddRow(current_row.getData(0));
				batch_to.AddRow(current_row.getData(batch_ti.getColsNo()));
			}


			back_prop(nn, g, batch_ti, batch_to);
			nn.Learn(g, rate);
			cost+= nn.Cost(batch_ti, batch_to);
			batch_begin += batch_size;
			if (batch_begin>=t.getRowsNo())
			{
				printf("avg cost:%f\n", cost / batch_count);
				++epoch;
				plot.push_back(cost/batch_count);
				cost = 0.0f;
				batch_begin = 0;

			}
			
		}
		BeginDrawing();
		ClearBackground(background_color);
		{



		int rw, rh, rx, ry;
		int w = GetRenderWidth();
		int h = GetRenderHeight();
		rw = w/ 2;
		rh = h* 2 / 3;
		rx = 0;
		ry = h/ 2 - rh / 2;
		plot_cost(plot, rx, ry, rw, rh);


		rw = w / 2;
		rh = h * 2 / 3;
		rx = w - rw;
		ry = h/ 2 - rh / 2;
		nn.RenderRaylib(rx,ry,rw,rh);
			
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "Epoch: %zu/%zu  Rate: %f", epoch,max_epoch,rate);

		DrawText(buffer, 0, 0, h * 0.05, WHITE);
		}
		EndDrawing();
	}
	return 0;
	for (size_t x = 0; x < n; ++x)
	{
		for (size_t y = 0; y < n; ++y) {
			std::cout << x << "+" << y<<"=";
			size_t z = x + y;
			for (size_t j = 0; j < BITS; ++j)
			{
				nn.m_activations[0][0][j] = (x >> j) & 1;
				nn.m_activations[0][0][j+BITS] = (y >> j) & 1;
			}
			nn.Forward();
			if (nn.m_activations[nn.m_count][0][BITS] > 0.5f)
			{
				std::cout << "OVERFLOW\n";
			}
			else
			{

			size_t a = 0;
			for (size_t j = 0; j < BITS; ++j)
			{
				size_t bit = nn.m_activations[nn.m_count][0][j] > 0.5f;
				a |= bit << j;
			}
			std::cout << a << "\n";
			}
		}
	}

		std::cin.get();
}