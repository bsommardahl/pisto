module Order = {
  type orderItem = {
    sku: string,
    name: string,
    suggestedPrice: Money.t,
    addedOn: float,
    salePrice: Money.t,
    taxCalculation: Tax.taxCalculationMethod,
  };
  type updateOrder = {
    id: string,
    customerName: string,
    orderItems: list(orderItem),
    discounts: list(Discount.t),
    amountPaid: option(Money.t),
    paymentTakenBy: option(string),
    paidOn: option(float),
  };
  type order = {
    id: string,
    customerName: string,
    amountPaid: option(Money.t),
    orderItems: list(orderItem),
    paymentTakenBy: option(string),
    paidOn: option(float),
    discounts: list(Discount.t),
    createdOn: float,
    lastUpdated: option(float),
    removed: bool,
  };
  type orderVm = {
    id: option(string),
    customerName: string,
    amountPaid: option(Money.t),
    orderItems: list(orderItem),
    paymentTakenBy: option(string),
    discounts: list(Discount.t),
    paidOn: option(float),
    createdOn: float,
    lastUpdated: option(float),
    removed: bool,
  };
  type newOrder = {
    customerName: string,
    orderItems: list(orderItem),
    discounts: list(Discount.t),
  };
};