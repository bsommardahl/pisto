module Order = {
  type orderItem = {
    product: Product.t,
    addedOn: float,
    salePrice: float,
  };
  type order = {
    id: option(string),
    customerName: string,
    amountPaid: option(float),
    orderItems: list(orderItem),
    paymentTakenBy: option(string),
    paidOn: option(float),
    createdOn: float,
  };
};