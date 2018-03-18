module Order = {
  type orderItem = {
    product: Product.t,
    addedOn: float,
    salePrice: float,
  };
  type order = {
    id: option(int),
    customerName: string,
    amountPaid: option(float),
    orderItems: list(orderItem),
    paymentTakenBy: option(string),
    paidOn: option(float),
    createdOn: float,
    lastUpdated: option(float),
    removed: bool,
  };
};