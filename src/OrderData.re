module Order = {
  type orderItem = {
    productCode: string,
    name: string,
    suggestedPrice: int,
    addedOn: float,
    salePrice: int,
    taxCalculation: Tax.taxCalculationMethod,
  };
  type updateOrder = {
    id: string,
    customerName: string,
    orderItems: list(orderItem),
    amountPaid: option(int),
    paymentTakenBy: option(string),
    paidOn: option(float),
  };
  type order = {
    id: string,
    customerName: string,
    amountPaid: option(int),
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
    amountPaid: option(int),
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