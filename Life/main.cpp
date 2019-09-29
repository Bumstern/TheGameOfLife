#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // ��������� �������
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

bool **create2D(const int n) {		// ������ ��������� ������
	bool **a = new bool *[n + 2];
	a[0] = new bool[(n + 2) * (n + 2)];
	for (int i = 1; i < n + 2; i++) {
		a[i] = a[i - 1] + n + 2;
	}
	return a;
}

void delete2D(bool **a) {
	delete[] a[0];
	a[0] = 0;
	delete[] a;
	a = 0;
}

int check(int i, int j, bool** first) {		// ������� ���-�� ����� ������ ������ (���. 1)
	int count = 0;
	for (int row = i - 1; row <= i + 1; ++row) {
		for (int col = j - 1; col <= j + 1; ++col) {
			if (first[row][col] == true) {
				++count;
			}
		}
	}
	if (first[i][j] == true) {
		--count;
	}
	return count;
	
	// . . .
	// . * .
	// . . .
	// ���. 1
}


int main() {
	const int N2D = 100, EDGE = 600, pix = EDGE / N2D;			// N2D - ������ 2D �������, EDGE - ������ ������� 2D �������, pix - ������ ������
	bool **first = create2D(N2D), **second = create2D(N2D);		// ��������� �������
	bool FlagStart = true, FlagMouse = false;					// ����� ��� ��������
	for (int i = 0; i < N2D + 2; i++) {							// ����������� ���������� ����-��
		for (int j = 0; j < N2D + 2; j++) {
			first[i][j] = false, second[i][j] = false;
		}
	}

	RenderWindow window(VideoMode(EDGE, EDGE), "The Game of Life",  Style::Close);	
	window.setFramerateLimit(15);
	
	RectangleShape rect(Vector2f(pix, pix)); // ����� ������
	RectangleShape dead(Vector2f(pix, pix)); // ������� ������
	rect.setFillColor(Color::Green);
	dead.setFillColor(Color::Transparent);
	dead.setOutlineThickness(-0.5);
	dead.setOutlineColor(Color(225, 225, 225, 45));
	
	//// ������ ����� ////
	for (int i = 1; i < N2D + 1; ++i) {
		for (int j = 1; j < N2D + 1; ++j) {
			dead.setPosition((i - 1) * pix, (j - 1) * pix);
			window.draw(dead);
		}
	}
	window.display();

	while (FlagStart) { 
		Event evnt;
		while (window.pollEvent(evnt)) {
			if (evnt.type == evnt.Closed) {
				delete2D(first), delete2D(second);
				window.close();
			}
			else if (evnt.type == evnt.KeyPressed && Keyboard::isKeyPressed(Keyboard::Enter)) {
				FlagStart = false;
				window.clear();
			}
			else if (evnt.type == sf::Event::MouseLeft) {
				FlagMouse = true;
			}
			else if (evnt.type == sf::Event::MouseEntered) {
				FlagMouse = false;
			}
				
				
		}

		//// ������������ ��������� ����� ////
		if (Mouse::isButtonPressed(Mouse::Left) && window.hasFocus() && FlagMouse == false) {
			first[(Mouse::getPosition(window).x / pix) + 1][(Mouse::getPosition(window).y / pix) + 1] = true;
		}
		
		for (int i = 1; i < N2D; i++) {		// ��� ����� - �� ����� � ��� �������������
			for (int j = 1; j < N2D; j++) {
				if (first[i][j] == true) {
					rect.setPosition((i - 1) * pix, (j - 1) * pix);
					window.draw(rect);
				}
				else {
					dead.setPosition((i - 1) * pix, (j - 1) * pix);
					window.draw(dead);
				}
			}
		}

		window.display();
		window.clear();		
	}	
	
	//// �������� ���� ////
	while (window.isOpen()) { 
		Event evnt;
		while (window.pollEvent(evnt)) {
			if (evnt.type == evnt.Closed) {
				delete2D(first), delete2D(second);
				window.close();
			}
		}

		for (int i = 1; i < N2D + 1; ++i) {		  // �������� �� �����/�������
			for (int j = 1; j < N2D + 1; ++j) {
				if (check(i, j, first) == 3 || (check(i, j, first) == 2 && first[i][j] == true)) {
					second[i][j] = true;
				}
				else {
					second[i][j] = false;
				}
			}
		}

		for (int i = 1; i < N2D + 1; ++i) {		//������ �� ������� ������
			for (int j = 1; j < N2D + 1; ++j) {
				first[i][j] = false;
				if (second[i][j] == true) {
					rect.setPosition((i - 1) * pix, (j - 1) * pix);
					window.draw(rect);
				}
				else {
					dead.setPosition((i - 1) * pix, (j - 1) * pix);
					window.draw(dead);
				}
			}
		}

		std::swap(first, second);	// ����� ��������	
		window.display();
		window.clear();
	}
}