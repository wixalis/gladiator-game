#include "stdafx.h"
#include "Converter.h"

double Converter::toMetres(int centimetres)
{
	return centimetres / 100.0;
}

double Converter::toKilograms(int grams)
{
	return grams / 1000.0;
}
