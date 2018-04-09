type t = {
  sku: string,
  name: string,
  suggestedPrice: Money.t,
  addedOn: float,
  salePrice: Money.t,
  taxCalculation: Tax.taxCalculationMethod,
};