module Order = {
  type orderItem = {
    productId: string,
    name: string,
    suggestedPrice: float,
    addedOn: float,
    salePrice: float,
  };
  type updateOrder = {
    id: string,
    customerName: string,
    orderItems: list(orderItem),
    amountPaid: option(float),
    paymentTakenBy: option(string),
    paidOn: option(float),
  };
  type order = {
    id: string,
    customerName: string,
    amountPaid: option(float),
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
    amountPaid: option(float),
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