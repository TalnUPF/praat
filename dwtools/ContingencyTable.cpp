/* ContingencyTable.cpp
 *
 * Copyright (C) 1993-2011 David Weenink
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "ContingencyTable.h"
#include "TableOfReal_extensions.h"
#include "NUM2.h"

Thing_implement (ContingencyTable, TableOfReal, 0);

#define TINY 1e-30

void structContingencyTable :: v_info () {
	structData :: v_info ();

	long ndf;
	double h, hx, hy, hygx, hxgy, uygx, uxgy, uxy, chisq;
	ContingencyTable_entropies (this, &h, &hx, &hy, &hygx, &hxgy, &uygx, &uxgy, &uxy);
	ContingencyTable_chisq (this, &chisq, &ndf);

	MelderInfo_writeLine (U"Number of rows: ", numberOfRows);
	MelderInfo_writeLine (U"Number of columns: ", numberOfColumns);
	MelderInfo_writeLine (U"Entropies (y is row variable):");
	MelderInfo_writeLine (U"  Total: ", h);
	MelderInfo_writeLine (U"  Y: ", hy);
	MelderInfo_writeLine (U"  X: ", hx);
	MelderInfo_writeLine (U"  Y given x: ", hygx);
	MelderInfo_writeLine (U"  X given y: ", hxgy);
	MelderInfo_writeLine (U"  Dependency of y on x: ", uygx);
	MelderInfo_writeLine (U"  Dependency of x on y: ", uxgy);
	MelderInfo_writeLine (U"  Symmetrical dependency: ", uxy);
	MelderInfo_writeLine (U"  Chi squared: ", chisq);
	MelderInfo_writeLine (U"  Degrees of freedom: ", ndf);
	MelderInfo_writeLine (U"  Probability: ", ContingencyTable_chisqProbability (this));
}

ContingencyTable ContingencyTable_create (long numberOfRows, long numberOfColumns) {
	try {
		autoContingencyTable me = Thing_new (ContingencyTable);
		TableOfReal_init (me.peek(), numberOfRows, numberOfColumns);
		return me.transfer();
	} catch (MelderError) {
		Melder_throw (U"ContingencyTable not created.");
	}
}

double ContingencyTable_chisqProbability (ContingencyTable me) {
	double chisq;
	long df;
	ContingencyTable_chisq (me, &chisq, &df);
	if (chisq == 0 && df == 0) {
		return 0;
	}
	return NUMchiSquareQ (chisq, df);
}

double ContingencyTable_cramersStatistic (ContingencyTable me) {
	double chisq, sum = 0;
	long df, nr = my numberOfRows, nc = my numberOfColumns, nmin = nr;

	if (nr == 1 || nc == 1) {
		return 0;
	}

	for (long i = 1; i <= nr; i++) {
		for (long j = 1; j <= nc; j++) {
			sum += my data[i][j];
		}
	}

	if (nc < nr) {
		nmin = nc;
	}
	nmin--;

	ContingencyTable_chisq (me, &chisq, &df);
	if (chisq == 0 && df == 0) {
		return 0;
	}
	return sqrt (chisq / (sum * nmin));
}

double ContingencyTable_contingencyCoefficient (ContingencyTable me) {
	double chisq, sum = 0;
	long df, nr = my numberOfRows, nc = my numberOfColumns;

	for (long i = 1; i <= nr; i++) {
		for (long j = 1; j <= nc; j++) {
			sum += my data[i][j];
		}
	}

	ContingencyTable_chisq (me, &chisq, &df);
	if (chisq == 0 && df == 0) {
		return 0;
	}
	return sqrt (chisq / (chisq + sum));
}

void ContingencyTable_chisq (ContingencyTable me, double *chisq, long *df) {
	long nr = my numberOfRows, nc = my numberOfColumns;

	*chisq = 0; *df = 0;

	autoNUMvector<double> rowsum (1, nr);
	autoNUMvector<double> colsum (1, nc);

	/*
		row and column marginals
	*/

	double sum = 0;
	for (long i = 1; i <= my numberOfRows; i++) {
		for (long j = 1; j <= my numberOfColumns; j++) {
			rowsum[i] += my data[i][j];
			colsum[j] += my data[i][j];
		}
		sum += rowsum[i];
	}

	for (long i = 1; i <= my numberOfRows; i++) {
		if (rowsum[i] == 0) {
			--nr;
		}
	}
	for (long j = 1; j <= my numberOfColumns; j++) {
		if (colsum[j] == 0) {
			--nc;
		}
	}

	*df = (nr - 1) * (nc - 1);
	for (long i = 1; i <= my numberOfRows; i++) {
		if (rowsum[i] == 0) {
			continue;
		}
		for (long j = 1; j <= my numberOfColumns; j++) {
			if (colsum[j] == 0) {
				continue;
			}
			double expt = rowsum[i] * colsum[j] / sum;
			double tmp = my data[i][j] - expt;
			*chisq += tmp * tmp / expt;
		}
	}
}

void ContingencyTable_entropies (ContingencyTable me, double *h, double *hx, double *hy,
                                 double *hygx, double *hxgy, double *uygx, double *uxgy, double *uxy) {
	*h = *hx = *hy = *hxgy = *hygx = *uygx = *uxgy = *uxy = 0;

	autoNUMvector<double> rowsum (1, my numberOfRows);
	autoNUMvector<double> colsum (1, my numberOfColumns);

	/*
		row and column totals
	*/

	double sum = 0.0;
	for (long i = 1; i <= my numberOfRows; i++) {
		for (long j = 1; j <= my numberOfColumns; j++) {
			rowsum[i] += my data[i][j];
			colsum[j] += my data[i][j];
		}
		sum += rowsum[i];
	}

	/*
		Entropy of x distribution
	*/

	for (long j = 1; j <= my numberOfColumns; j++) {
		if (colsum[j] > 0) {
			double p = colsum[j] / sum;
			*hx -= p * NUMlog2 (p);
		}
	}

	/*
		Entropy of y distribution
	*/

	for (long i = 1; i <= my numberOfRows; i++) {
		if (rowsum[i] > 0) {
			double p = rowsum[i] / sum;
			*hy -= p * NUMlog2 (p);
		}
	}

	/*
		Total entropy
	*/

	for (long i = 1; i <= my numberOfRows; i++) {
		for (long j = 1; j <= my numberOfColumns; j++) {
			if (my data[i][j] > 0) {
				double p = my data[i][j] / sum;
				*h -= p * NUMlog2 (p);
			}
		}
	}

	/*
		Conditional entropies
	*/

	*hygx = *h - *hx;
	*hxgy = *h - *hy;
	*uygx = (*hy - *hygx) / (*hy + TINY);
	*uxgy = (*hx - *hxgy) / (*hx + TINY);
	*uxy = 2.0 * (*hx + *hy - *h) / (*hx + *hy + TINY);
}


ContingencyTable Confusion_to_ContingencyTable (Confusion me) {
	try {
		autoContingencyTable thee = Thing_new (ContingencyTable);
		my structTableOfReal :: v_copy (thee.peek());
		return thee.transfer();
	} catch (MelderError) {
		Melder_throw (me, U": not converted to ContingencyTable.");
	}
}

ContingencyTable TableOfReal_to_ContingencyTable (I) {
	iam (TableOfReal);
	try {
		TableOfReal_checkPositive (me);
		autoContingencyTable thee = Thing_new (ContingencyTable);
		my structTableOfReal :: v_copy (thee.peek());
		return thee.transfer();
	} catch (MelderError) {
		Melder_throw (me, U": not converted to ContingencyTable.");
	}
}

// End of file ContingencyTable.cpp
