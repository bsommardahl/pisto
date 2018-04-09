type updateOrder = {
  id: string,
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  paid: option(Paid.t),
};

type t = {
  id: string,
  customerName: string,
  paid: option(Paid.t),
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  createdOn: float,
  lastUpdated: option(float),
  removed: bool,
};

type orderVm = {
  id: option(string),
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  paid: option(Paid.t),
  createdOn: float,
  lastUpdated: option(float),
  removed: bool,
};

type newOrder = {
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
};