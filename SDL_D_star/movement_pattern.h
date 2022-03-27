#ifndef MOVEMENT_PATTERN_H_INCLUDED
#define MOVEMENT_PATTERN_H_INCLUDED

//felfele mozg�s halad�s blokkol�sai
bool straight_matrix_M15[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool straight_matrix_M10[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,0,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool straight_matrix_M5[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,0,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},

{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,1,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool straight_matrix_0[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,0,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},

{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool straight_matrix_P5[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool straight_matrix_P10[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_M15[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_M10[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_M5[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,0,0, 1,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,0, 0,0,0, 0,0,0},

{0,0,1, 0,0,1, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,1,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_0[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,1,0, 1,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,0,0, 1,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,0, 0,0,0, 0,0,0},

{0,0,1, 0,1,1, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_P5[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,1,0, 1,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,1,1, 1,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,1,1, 1,0,0, 0,0,0, 0,0,0},

{0,0,1, 0,1,1, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_P10[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,1,0, 1,0,0, 0,0,0, 0,0,0},

{0,0,0, 1,1,1, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 1,1,1, 1,0,0, 0,0,0, 0,0,0},
{0,0,0, 1,1,1, 0,0,0, 0,0,0, 0,0,0},

{0,0,1, 0,1,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool slash_matrix_P15[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 0,1,0, 1,0,0, 0,0,0, 0,0,0},

{0,0,0, 1,1,1, 0,0,0, 0,0,0, 0,0,0},
{0,0,1, 1,1,1, 1,0,0, 0,0,0, 0,0,0},
{0,0,0, 1,1,1, 0,0,0, 0,0,0, 0,0,0},

{0,0,1, 0,1,0, 1,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool fal_matrix[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},

{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},

{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

bool fal_matrix_kerek[15][15] =
{
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},

{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},
{0,0,0, 0,0,1, 1,1,1, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 1,1,1, 0,0,0, 0,0,0},

{0,0,0, 0,0,1, 0,1,0, 1,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0}
};

#pragma GCC optimize ("O0")
bool matrix_rotate(bool matrix[15][15]){ // balra
    bool ret[15][15];
    for (int i=0; i<15; i++){
        for (int j=0; j<15; j++){
            ret[14-j][i] = matrix[i][j];
        }
    }
    for (int i=0; i<15; i++){
        for (int j=0; j<15; j++){
            matrix[i][j] = ret[i][j];
        }
    }
}

bool matrix_cpy(bool from[15][15], bool to[15][15]){

    for (int i=0; i<15*15; i++){
        //cout<<i<<" "<<to[i/15][i%15]<<" "<<from[i/15][i%15]<<endl;;
        to[i/15][i%15]=from[i/15][i%15];
    }
}

#endif // MOVEMENT_PATTERN_H_INCLUDED
