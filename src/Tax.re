type taxCalculationMethod =
  | Exempt
  | TotalFirst(int)
  | SubTotalFirst(int);