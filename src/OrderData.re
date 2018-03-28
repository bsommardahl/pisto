module Order = {
  type orderItem = {
    productCode: string,
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
    paidOn: option(float),
    createdOn: float,
    lastUpdated: option(float),
    removed: bool,
  };
  type newOrder = {
    customerName: string,
    orderItems: list(orderItem),
  };
};